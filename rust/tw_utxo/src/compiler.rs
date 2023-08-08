use crate::{Error, Result};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::consensus::Encodable;
use bitcoin::hashes::Hash;
use bitcoin::sighash::{EcdsaSighashType, Prevouts, SighashCache, TapSighashType};
use bitcoin::taproot::TapLeafHash;
use bitcoin::{OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness};
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_proto::Utxo::Proto::{self, SighashType};

type ProtoLockTimeVariant = Proto::mod_SigningInput::OneOflock_time;
type ProtoSigningMethod<'a> = Proto::mod_TxIn::OneOfsighash_method<'a>;

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
    pub fn preimage_hashes(proto: &Proto::SigningInput<'_>) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(proto)
            .or_else(|err| {
                std::result::Result::<_, ()>::Ok(Proto::PreSigningOutput {
                    error: err.into(),
                    sighashes: Default::default(),
                })
            })
            .expect("did not convert error value")
    }

    #[inline]
    pub fn compile(proto: &Proto::PreSerialization<'_>) -> Proto::SerializedTransaction<'static> {
        Self::compile_impl(proto)
            .or_else(|err| {
                std::result::Result::<_, ()>::Ok(Proto::SerializedTransaction {
                    error: err.into(),
                    encoded: Default::default(),
                })
            })
            .expect("did not convert error value")
    }

    fn preimage_hashes_impl(
        proto: &Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        // Convert Protobuf structure to `bitcoin` crate native transaction.
        // Prepare signing mechanism.
        let tx = convert_proto_to_tx(&proto)?;
        let mut cache = SighashCache::new(&tx);

        let mut sighashes: Vec<Vec<u8>> = vec![];

        for (index, input) in proto.inputs.iter().enumerate() {
            match input.sighash_method {
                // Use the legacy hashing mechanism (e.g. P2SH, P2PK, P2PKH).
                ProtoSigningMethod::legacy(ref legacy) => {
                    let script_pubkey = Script::from_bytes(legacy.script_pubkey.as_ref());
                    let sighash_type = if let SighashType::UseDefault = input.sighash {
                        EcdsaSighashType::All
                    } else {
                        EcdsaSighashType::from_consensus(input.sighash as u32)
                    };

                    let sighash =
                        cache.legacy_signature_hash(index, script_pubkey, sighash_type.to_u32())?;

                    sighashes.push(sighash.as_byte_array().to_vec());
                },
                // Use the Segwit hashing mechanism (e.g. P2WSH, P2WPKH).
                ProtoSigningMethod::segwit(ref segwit) => {
                    let script_pubkey = ScriptBuf::from_bytes(segwit.script_pubkey.to_vec());
                    let sighash_type = if let SighashType::UseDefault = input.sighash {
                        EcdsaSighashType::All
                    } else {
                        EcdsaSighashType::from_consensus(input.sighash as u32)
                    };
                    let value = segwit.value;

                    let sighash = cache.segwit_signature_hash(
                        index,
                        script_pubkey
                            .p2wpkh_script_code()
                            .as_ref()
                            .ok_or(Error::from(Proto::Error::Error_invalid_wpkh_script_pubkey))?,
                        value,
                        sighash_type,
                    )?;

                    sighashes.push(sighash.as_byte_array().to_vec());
                },
                // Use the Taproot hashing mechanism (e.g. P2TR key-path/script-path)
                ProtoSigningMethod::taproot(ref taproot) => {
                    let leaf_hash = if taproot.leaf_hash.is_empty() {
                        None
                    } else {
                        Some((
                            TapLeafHash::from_slice(taproot.leaf_hash.as_ref())
                                .map_err(|_| Error::from(Proto::Error::Error_invalid_leaf_hash))?,
                            // TODO: We might want to make this configurable.
                            0xFFFFFFFF,
                        ))
                    };

                    // Note that `input.sighash = 0` is handled by the underlying library.
                    let sighash_type = TapSighashType::from_consensus_u8(input.sighash as u8)
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_sighash_type))?;

                    // This owner only exists to avoid running into lifetime
                    // issues related to `Prevouts::All(&[T])`.
                    let _owner;

                    let prevouts: Prevouts<'_, TxOut> = match taproot.prevout {
                        Proto::mod_TxIn::mod_Taproot::OneOfprevout::one(one) => {

                            Prevouts::One(
                                index,
                                TxOut {
                                    value: one.value,
                                    script_pubkey: ScriptBuf::from_bytes(one.script_pubkey.to_vec()),
                                },
                            )
                        },
                        Proto::mod_TxIn::mod_Taproot::OneOfprevout::all(prevouts) => {
                            _owner = Some(
                                prevouts
                                    .all
                                    .iter()
                                    .map(|p| {
                                        TxOut {
                                            value: p.value,
                                            script_pubkey: ScriptBuf::from_bytes(p.script_pubkey.to_vec()),
                                        }
                                    })
                                    .collect::<Vec<TxOut>>(),
                            );

                            Prevouts::All(_owner.as_ref().expect("_owner not initialized"))
                        },
                        Proto::mod_TxIn::mod_Taproot::OneOfprevout::None => {
                            panic!();
                        }
                    };

                    dbg!(&prevouts);

                    let sighash = cache.taproot_signature_hash(
                        index,
                        &prevouts,
                        None,
                        leaf_hash,
                        sighash_type,
                    )?;

                    sighashes.push(sighash.as_byte_array().to_vec());
                },
                ProtoSigningMethod::None => {
                    return Err(Error::from(Proto::Error::Error_missing_sighash_method))
                },
            }
        }

        Ok(Proto::PreSigningOutput {
            error: Proto::Error::OK,
            sighashes: sighashes.into_iter().map(Cow::Owned).collect(),
        })
    }

    fn compile_impl(
        proto: &Proto::PreSerialization<'_>,
    ) -> Result<Proto::SerializedTransaction<'static>> {
        type ProtoLockTimeVariant = Proto::mod_PreSerialization::OneOflock_time;

        let version = proto.version;

        // Retreive the lock time. If none is provided, the default lock time is
        // used (immediately spendable).
        let lock_time = match proto.lock_time {
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
        };

        let mut tx = Transaction {
            version,
            lock_time,
            input: vec![],
            output: vec![],
        };

        for txin in &proto.inputs {
            let txid = Txid::from_slice(txin.txid.as_ref())
                .map_err(|_| Error::from(Proto::Error::Error_invalid_txid))?;
            let vout = txin.vout;
            let sequence = Sequence::from_consensus(txin.sequence);
            let script_sig = ScriptBuf::from_bytes(txin.script_sig.to_vec());
            let witness = if let Some(witness) = txin.witness.as_ref() {
                Witness::from_slice(
                    &witness
                        .items
                        .iter()
                        .map(|s| s.as_ref())
                        .collect::<Vec<&[u8]>>(),
                )
            } else {
                Witness::new()
            };

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

        Ok(Proto::SerializedTransaction {
            error: Proto::Error::OK,
            encoded: buffer.into(),
        })
    }
}

fn convert_proto_to_tx<'a>(proto: &Proto::SigningInput<'a>) -> Result<Transaction> {
    let version = proto.version;

    // Retreive the lock time. If none is provided, the default lock time is
    // used (immediately spendable).
    let lock_time = match proto.lock_time {
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
    };

    let mut tx = Transaction {
        version,
        lock_time,
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
            // TODO: This is actually important for signing, add as field in Utxo.
            sequence: Sequence::default(),
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
