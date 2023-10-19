use crate::{Error, Result};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::consensus::Encodable;
use bitcoin::hashes::Hash;
use bitcoin::sighash::{EcdsaSighashType, Prevouts, SighashCache, TapSighashType};
use bitcoin::taproot::TapLeafHash;
use bitcoin::{OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness};
use std::marker::PhantomData;
use tw_proto::Utxo::Proto::{self, SighashType};

type ProtoLockTimeVariant = Proto::mod_LockTime::OneOfvariant;
type ProtoSigningMethod = Proto::SigningMethod;

pub trait UtxoContext {
    type SigningInput<'a>;
    type SigningOutput;
    type PreSigningOutput;
}

pub struct StandardBitcoinContext;

impl UtxoContext for StandardBitcoinContext {
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningInput<'static>;
    type PreSigningOutput = Proto::SigningInput<'static>;
}

pub struct Compiler<Context: UtxoContext> {
    _phantom: PhantomData<Context>,
}

impl Compiler<StandardBitcoinContext> {
    #[inline]
    pub fn preimage_hashes(proto: Proto::SigningInput<'_>) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(proto)
            .or_else(|err| {
                std::result::Result::<_, ()>::Ok(Proto::PreSigningOutput {
                    error: err.into(),
                    ..Default::default()
                })
            })
            .expect("did not convert error value")
    }

    #[inline]
    pub fn compile(proto: Proto::PreSerialization<'_>) -> Proto::SerializedTransaction<'static> {
        Self::compile_impl(proto)
            .or_else(|err| {
                std::result::Result::<_, ()>::Ok(Proto::SerializedTransaction {
                    error: err.into(),
                    ..Default::default()
                })
            })
            .expect("did not convert error value")
    }

    fn preimage_hashes_impl(
        mut proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        // TODO: Check for duplicate Txid (user error).

        // Calculate total outputs amount, based on it we can determine how many inputs to select.
        let total_input: u64 = proto.inputs.iter().map(|input| input.value).sum();
        let total_output: u64 = proto.outputs.iter().map(|output| output.value).sum();

        // Do some easy checks first.

        // Insufficient input amount.
        if total_output > total_input {
            return Err(Error::from(Proto::Error::Error_insufficient_inputs));
        }

        // Change scriptPubkey must be set if change output is enabled.
        if !proto.disable_change_output && proto.change_script_pubkey.is_empty() {
            return Err(Error::from(
                Proto::Error::Error_missing_change_script_pubkey,
            ));
        }

        // If the input selector is InputSelector::SelectAscending, we sort the
        // input first.
        if let Proto::InputSelector::SelectAscending = proto.input_selector {
            proto.inputs.sort_by(|a, b| a.value.cmp(&b.value));
        }

        // Unless InputSelector::UseAll is provided, we only use the necessariy
        // amount of inputs to cover `total_output`. Any other input gets
        // dropped.
        let selected = if let Proto::InputSelector::SelectInOrder
        | Proto::InputSelector::SelectAscending = proto.input_selector
        {
            let mut total_input = total_input;
            let mut remaining = total_output;

            let selected: Vec<Proto::TxIn> = proto
                .inputs
                .into_iter()
                .take_while(|input| {
                    if remaining == 0 {
                        return false;
                    }

                    total_input += input.value;
                    remaining = remaining.saturating_sub(input.value);

                    true
                })
                .map(|input| Proto::TxIn {
                    txid: input.txid.to_vec().into(),
                    script_pubkey: input.script_pubkey.to_vec().into(),
                    leaf_hash: input.leaf_hash.to_vec().into(),
                    ..input
                })
                .collect();

            selected
        } else {
            // TODO: Write a function for this
            proto
                .inputs
                .into_iter()
                .map(|input| Proto::TxIn {
                    txid: input.txid.to_vec().into(),
                    script_pubkey: input.script_pubkey.to_vec().into(),
                    leaf_hash: input.leaf_hash.to_vec().into(),
                    ..input
                })
                .collect()
        };

        // Update protobuf structure with selected inputs.
        proto.inputs = selected.clone();

        // Update the `total_input` amount based on the selected inputs.
        let total_input: u64 = proto.inputs.iter().map(|input| input.value).sum();

        // Calculate the total input weight projection.
        let input_weight: u64 = proto.inputs.iter().map(|input| input.weight_estimate).sum();

        // Convert Protobuf structure to `bitcoin` crate native transaction,
        // used for weight/fee calculation.
        let tx = convert_proto_to_tx(&proto)?;

        // Estimate of the change output weight.
        let output_weight = if proto.disable_change_output {
            0
        } else {
            // VarInt + script_pubkey size, rough estimate.
            1 + proto.change_script_pubkey.len() as u64
        };

        // Calculate the full weight projection (base weight + input & output weight).
        let weight_estimate = tx.weight().to_wu() + input_weight + output_weight;
        let fee_estimate = (weight_estimate + 3) / 4 * proto.weight_base;

        // Check if the fee projection would make the change amount negative
        // (implying insufficient input amount).
        let change_amount_before_fee = total_input - total_output;
        if change_amount_before_fee < fee_estimate {
            return Err(Error::from(Proto::Error::Error_insufficient_inputs));
        }

        if !proto.disable_change_output {
            // The amount to be returned (if enabled).
            let change_amount = change_amount_before_fee - fee_estimate;

            // Update the passed on protobuf structure by adding a change output
            // (return to sender)
            if change_amount != 0 {
                proto.outputs.push(Proto::TxOut {
                    value: change_amount,
                    script_pubkey: proto.change_script_pubkey.clone(),
                });
            }
        }

        // Convert *updated* Protobuf structure to `bitcoin` crate native
        // transaction.
        let tx = convert_proto_to_tx(&proto)?;

        let mut cache = SighashCache::new(&tx);

        let mut sighashes: Vec<(Vec<u8>, ProtoSigningMethod, Proto::SighashType)> = vec![];

        for (index, input) in proto.inputs.iter().enumerate() {
            match input.signing_method {
                // Use the legacy hashing mechanism (e.g. P2SH, P2PK, P2PKH).
                ProtoSigningMethod::Legacy => {
                    let script_pubkey = Script::from_bytes(input.script_pubkey.as_ref());
                    let sighash_type = if let SighashType::UseDefault = input.sighash_type {
                        EcdsaSighashType::All
                    } else {
                        EcdsaSighashType::from_consensus(input.sighash_type as u32)
                    };
                    let sighash =
                        cache.legacy_signature_hash(index, script_pubkey, sighash_type.to_u32())?;

                    sighashes.push((
                        sighash.as_byte_array().to_vec(),
                        ProtoSigningMethod::Legacy,
                        input.sighash_type,
                    ));
                },
                // Use the Segwit hashing mechanism (e.g. P2WSH, P2WPKH).
                ProtoSigningMethod::Segwit => {
                    let script_pubkey = ScriptBuf::from_bytes(input.script_pubkey.to_vec());
                    let sighash_type = if let SighashType::UseDefault = input.sighash_type {
                        EcdsaSighashType::All
                    } else {
                        EcdsaSighashType::from_consensus(input.sighash_type as u32)
                    };

                    let sighash = cache.segwit_signature_hash(
                        index,
                        script_pubkey.as_script(),
                        input.value,
                        sighash_type,
                    )?;

                    sighashes.push((
                        sighash.as_byte_array().to_vec(),
                        ProtoSigningMethod::Segwit,
                        input.sighash_type,
                    ));
                },
                // Use the Taproot hashing mechanism (e.g. P2TR key-path/script-path)
                ProtoSigningMethod::TaprootAll => {
                    let leaf_hash = if input.leaf_hash.is_empty() {
                        None
                    } else {
                        Some((
                            TapLeafHash::from_slice(input.leaf_hash.as_ref())
                                .map_err(|_| Error::from(Proto::Error::Error_invalid_leaf_hash))?,
                            // TODO: We might want to make this configurable?.
                            0xFFFFFFFF,
                        ))
                    };

                    // Note that `input.sighash_type = 0` is handled by the underlying library.
                    let sighash_type = TapSighashType::from_consensus_u8(input.sighash_type as u8)
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_sighash_type))?;

                    let prevouts = proto
                        .inputs
                        .iter()
                        .map(|i| TxOut {
                            value: i.value,
                            script_pubkey: ScriptBuf::from_bytes(i.script_pubkey.to_vec()),
                        })
                        .collect::<Vec<TxOut>>();

                    let sighash = cache.taproot_signature_hash(
                        index,
                        &Prevouts::All(&prevouts),
                        None,
                        leaf_hash,
                        sighash_type,
                    )?;

                    sighashes.push((
                        sighash.as_byte_array().to_vec(),
                        ProtoSigningMethod::TaprootAll,
                        input.sighash_type,
                    ));
                },
                ProtoSigningMethod::TaprootOnePrevout => {
                    let leaf_hash = if input.leaf_hash.is_empty() {
                        None
                    } else {
                        Some((
                            TapLeafHash::from_slice(input.leaf_hash.as_ref())
                                .map_err(|_| Error::from(Proto::Error::Error_invalid_leaf_hash))?,
                            // TODO: We might want to make this configurable?.
                            0xFFFFFFFF,
                        ))
                    };

                    // Note that `input.sighash_type = 0` is handled by the underlying library.
                    let sighash_type = TapSighashType::from_consensus_u8(input.sighash_type as u8)
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_sighash_type))?;

                    let prevouts = Prevouts::One(
                        index,
                        TxOut {
                            value: input.value,
                            script_pubkey: ScriptBuf::from_bytes(input.script_pubkey.to_vec()),
                        },
                    );

                    let sighash = cache.taproot_signature_hash(
                        index,
                        &prevouts,
                        None,
                        leaf_hash,
                        sighash_type,
                    )?;

                    sighashes.push((
                        sighash.as_byte_array().to_vec(),
                        ProtoSigningMethod::TaprootOnePrevout,
                        input.sighash_type,
                    ));
                },
            }
        }

        let tx = cache.into_transaction();
        // The transaction identifier, which we represent in
        // non-reversed/non-network order.
        let txid: Vec<u8> = tx.txid().as_byte_array().iter().copied().rev().collect();

        Ok(Proto::PreSigningOutput {
            error: Proto::Error::OK,
            txid: txid.into(),
            sighashes: sighashes
                .into_iter()
                .map(|(sighash, method, sighash_type)| Proto::Sighash {
                    sighash: sighash.into(),
                    signing_method: method,
                    sighash_type,
                })
                .collect(),
            inputs: selected,
            outputs: proto
                .outputs
                .into_iter()
                .map(|output| Proto::TxOut {
                    value: output.value,
                    script_pubkey: output.script_pubkey.to_vec().into(),
                })
                .collect(),
            weight_estimate,
            fee_estimate,
        })
    }

    fn compile_impl(
        proto: Proto::PreSerialization<'_>,
    ) -> Result<Proto::SerializedTransaction<'static>> {
        let mut tx = Transaction {
            version: proto.version,
            lock_time: lock_time_from_proto(&proto.lock_time)?,
            input: vec![],
            output: vec![],
        };

        for txin in &proto.inputs {
            let txid = Txid::from_slice(txin.txid.as_ref())
                .map_err(|_| Error::from(Proto::Error::Error_invalid_txid))?;
            let vout = txin.vout;
            let sequence = Sequence::from_consensus(txin.sequence);
            let script_sig = ScriptBuf::from_bytes(txin.script_sig.to_vec());
            let witness = Witness::from_slice(
                &txin
                    .witness_items
                    .iter()
                    .map(|s| s.as_ref())
                    .collect::<Vec<&[u8]>>(),
            );

            tx.input.push(TxIn {
                previous_output: OutPoint { txid, vout },
                script_sig,
                sequence,
                witness,
            });
        }

        for txout in &proto.outputs {
            tx.output.push(TxOut {
                value: txout.value,
                script_pubkey: ScriptBuf::from_bytes(txout.script_pubkey.to_vec()),
            });
        }

        // Encode the transaction.
        let mut buffer = vec![];
        tx.consensus_encode(&mut buffer)
            .map_err(|_| Error::from(Proto::Error::Error_failed_encoding))?;

        // The transaction identifier, which we represent in
        // non-reversed/non-network order.
        let txid: Vec<u8> = tx.txid().as_byte_array().iter().copied().rev().collect();

        Ok(Proto::SerializedTransaction {
            error: Proto::Error::OK,
            encoded: buffer.into(),
            txid: txid.into(),
            weight: tx.weight().to_wu(),
            fee: tx.weight().to_vbytes_ceil() * proto.weight_base,
        })
    }
}

fn convert_proto_to_tx<'a>(proto: &'a Proto::SigningInput<'a>) -> Result<Transaction> {
    let mut tx = Transaction {
        version: proto.version,
        lock_time: lock_time_from_proto(&proto.lock_time)?,
        input: vec![],
        output: vec![],
    };

    for txin in &proto.inputs {
        let txid = Txid::from_slice(txin.txid.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_txid))?;

        let vout = txin.vout;

        tx.input.push(TxIn {
            previous_output: OutPoint { txid, vout },
            script_sig: ScriptBuf::new(),
            sequence: Sequence(txin.sequence),
            witness: Witness::new(),
        });
    }

    for txout in &proto.outputs {
        tx.output.push(TxOut {
            value: txout.value,
            script_pubkey: ScriptBuf::from_bytes(txout.script_pubkey.to_vec()),
        });
    }

    Ok(tx)
}

// Convenience function to retreive the lock time. If none is provided, the
// default lock time is used (immediately spendable).
fn lock_time_from_proto(proto: &Option<Proto::LockTime>) -> Result<LockTime> {
    let lock_time = if let Some(lock_time) = proto {
        match lock_time.variant {
            ProtoLockTimeVariant::blocks(block) => LockTime::Blocks(
                Height::from_consensus(block)
                    .map_err(|_| Error::from(Proto::Error::Error_invalid_lock_time))?,
            ),
            ProtoLockTimeVariant::seconds(secs) => LockTime::Seconds(
                Time::from_consensus(secs)
                    .map_err(|_| Error::from(Proto::Error::Error_invalid_lock_time))?,
            ),
            ProtoLockTimeVariant::None => LockTime::Blocks(
                Height::from_consensus(0)
                    .map_err(|_| Error::from(Proto::Error::Error_invalid_lock_time))?,
            ),
        }
    } else {
        LockTime::Blocks(
            Height::from_consensus(0)
                .map_err(|_| Error::from(Proto::Error::Error_invalid_lock_time))?,
        )
    };

    Ok(lock_time)
}
