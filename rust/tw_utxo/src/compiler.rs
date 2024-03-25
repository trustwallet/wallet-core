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
        // Calculate total outputs amount, based on it we can determine how many inputs to select.
        let total_input_amount: u64 = proto.inputs.iter().map(|input| input.value).sum();
        let total_output_amount: u64 = proto.outputs.iter().map(|output| output.value).sum();

        // Do some easy checks first.

        // Insufficient input amount.
        if total_output_amount > total_input_amount {
            return Err(Error::from(Proto::Error::Error_insufficient_inputs));
        }

        // No ouputs specified.
        if total_output_amount == 0 {
            return Err(Error::from(Proto::Error::Error_no_outputs_specified));
        }

        // Change scriptPubkey must be set if change output is enabled.
        if !proto.disable_change_output && proto.change_script_pubkey.is_empty() {
            return Err(Error::from(
                Proto::Error::Error_missing_change_script_pubkey,
            ));
        }

        // If enabled, sort the order of the UTXOs.
        if let Proto::InputSelector::SelectAscending = proto.input_selector {
            proto.inputs.sort_by(|a, b| a.value.cmp(&b.value));
        } else if let Proto::InputSelector::SelectDescending = proto.input_selector {
            proto.inputs.sort_by(|a, b| b.value.cmp(&a.value));
        }

        // Add change output generation is enabled, push it to the proto structure.
        if !proto.disable_change_output {
            proto.outputs.push(Proto::TxOut {
                // We set the change value later.
                value: 0,
                script_pubkey: proto.change_script_pubkey,
            });
        }

        // Prepare the `bitcoin` crate native transaction structure, used for fee calculation.
        let mut tx = Transaction {
            version: proto.version,
            lock_time: lock_time_from_proto(&proto.lock_time)?,
            // Leave inputs empty for now.
            input: vec![],
            // Add outputs (including change output)
            output: proto
                .outputs
                .iter()
                .map(|output| {
                    // Conver to `bitcoin` crate native type.
                    TxOut {
                        value: output.value,
                        script_pubkey: ScriptBuf::from_bytes(output.script_pubkey.to_vec()),
                    }
                })
                .collect(),
        };

        // Select the inputs accordingly by updating `proto.inputs`.
        let available = std::mem::take(&mut proto.inputs); // Drain `proto.inputs`
        match &proto.input_selector {
            Proto::InputSelector::UseAll => {
                // Simply add all inputs.
                for txin in available {
                    let n_txin = convert_proto_to_txin(&txin)?;
                    tx.input.push(n_txin);

                    // Track selected input
                    proto.inputs.push(txin);
                }
            },
            Proto::InputSelector::SelectInOrder
            | Proto::InputSelector::SelectAscending
            | Proto::InputSelector::SelectDescending => {
                let mut total_input_amount = 0;
                let mut total_input_weight = 0;

                // For each iteration, we calculate the full fee estimate and
                // exit when the total amount + fees have been covered.
                for txin in available {
                    let n_txin = convert_proto_to_txin(&txin)?;
                    tx.input.push(n_txin);

                    // Update input amount and weight.
                    total_input_amount += txin.value;
                    total_input_weight += txin.weight_estimate; // contains scriptSig/Witness weight

                    // Track selected input
                    proto.inputs.push(txin);

                    // Update the change amount, if set.
                    if !proto.disable_change_output {
                        let change_output = tx.output.last_mut().expect("change output not set");
                        change_output.value =
                            total_input_amount.saturating_sub(total_output_amount);
                    }

                    // Calculate the full weight projection (base weight + input
                    // weight + output weight). Note that the change output itself is
                    // not included in the transaction yet.
                    let weight_estimate = tx.weight().to_wu() + total_input_weight;
                    let fee_estimate = (weight_estimate + 3) / 4 * proto.weight_base;

                    if total_input_amount >= total_output_amount + fee_estimate {
                        // Enough inputs to cover the output and fee estimate.
                        break;
                    }
                }
            },
        };

        // Update the `total input amount based on the selected inputs.
        let total_input_amount: u64 = proto.inputs.iter().map(|input| input.value).sum();

        // Calculate the total input weight projection.
        let total_input_weight: u64 = proto.inputs.iter().map(|input| input.weight_estimate).sum();

        // Calculate the weight projection (base weight + input weight + output
        // weight). Note that the scriptSig/Witness fields are blanked inside
        // `tx`, hence we need to rely on the values passed on the proto
        // structure.
        let weight_estimate = tx.weight().to_wu() + total_input_weight;
        let fee_estimate = (weight_estimate + 3) / 4 * proto.weight_base;

        // Check if there are enough inputs to cover the full output and fee estimate.
        if total_input_amount < total_output_amount + fee_estimate {
            return Err(Error::from(Proto::Error::Error_insufficient_inputs));
        }

        // Set the change output amount in the proto structure, if enabled.
        if !proto.disable_change_output {
            // Update the change amount in the proto list.
            let change_output = proto.outputs.last_mut().expect("change output not set");
            change_output.value = total_input_amount
                .saturating_sub(total_output_amount)
                .saturating_sub(fee_estimate);

            // Update the change amount in the `bitcoin` crate native transaction.
            // This is required for the sighash calculation.
            tx.output.last_mut().expect("change output not set").value = change_output.value;
        }

        // Calculate the effective fee.
        let total_output_amount: u64 = proto.outputs.iter().map(|out| out.value).sum();
        let fee_estimate = total_input_amount - total_output_amount;

        // Calculate the sighashes.
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
            inputs: proto
                .inputs
                .into_iter()
                .map(|input| Proto::TxIn {
                    txid: input.txid.to_vec().into(),
                    vout: input.vout,
                    sequence: input.sequence,
                    value: input.value,
                    script_pubkey: input.script_pubkey.to_vec().into(),
                    weight_estimate: input.weight_estimate,
                    signing_method: input.signing_method,
                    sighash_type: input.sighash_type,
                    leaf_hash: input.leaf_hash.to_vec().into(),
                })
                .collect(),
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
        // Do some easy checks first.

        let total_input_amount: u64 = proto.inputs.iter().map(|input| input.value).sum();
        let total_output_amount: u64 = proto.outputs.iter().map(|output| output.value).sum();

        // Insufficient input amount.
        if total_output_amount > total_input_amount {
            return Err(Error::from(Proto::Error::Error_insufficient_inputs));
        }

        // No ouputs specified.
        if total_output_amount == 0 {
            return Err(Error::from(Proto::Error::Error_no_outputs_specified));
        }

        let mut tx = Transaction {
            version: proto.version,
            lock_time: lock_time_from_proto(&proto.lock_time)?,
            input: vec![],
            output: vec![],
        };

        let mut total_input_amount = 0;
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

            total_input_amount += txin.value;

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

        // Sanity check.
        debug_assert_eq!(tx.input.len(), proto.inputs.len());
        debug_assert_eq!(tx.output.len(), proto.outputs.len());

        // Encode the transaction.
        let mut buffer = vec![];
        tx.consensus_encode(&mut buffer)
            .map_err(|_| Error::from(Proto::Error::Error_failed_encoding))?;

        // The transaction identifier, which we represent in
        // non-reversed/non-network order.
        let txid: Vec<u8> = tx.txid().as_byte_array().iter().copied().rev().collect();
        let weight = tx.weight().to_wu();

        // Calculate the effective fee.
        let total_output_amount = tx.output.iter().map(|out| out.value).sum::<u64>();
        debug_assert!(total_input_amount >= total_output_amount);
        let fee = total_input_amount - total_output_amount;

        Ok(Proto::SerializedTransaction {
            error: Proto::Error::OK,
            encoded: buffer.into(),
            txid: txid.into(),
            weight,
            fee,
        })
    }
}

fn convert_proto_to_txin<'a>(proto: &'a Proto::TxIn<'a>) -> Result<TxIn> {
    let txid = Txid::from_slice(proto.txid.as_ref())
        .map_err(|_| Error::from(Proto::Error::Error_invalid_txid))?;

    let vout = proto.vout;

    Ok(TxIn {
        previous_output: OutPoint { txid, vout },
        // Utxo.proto does not have the field, we rely on
        // `Proto::TxIn.weight_estimate` for estimating fees.
        script_sig: ScriptBuf::new(),
        sequence: Sequence(proto.sequence),
        // Utxo.proto does not have the field, we rely on
        // `Proto::TxIn.weight_estimate` for estimating fees.
        witness: Witness::new(),
    })
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
