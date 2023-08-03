use crate::{Error, Result};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::hashes::Hash;
use bitcoin::sighash::{EcdsaSighashType, Prevouts, SighashCache, TapSighashType};
use bitcoin::taproot::TapLeafHash;
use bitcoin::{OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness};
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_proto::Utxo::Proto::{self, SighashType};

type ProtoLockTimeVariant = Proto::mod_SigningInput::OneOflock_time;
type ProtoSigningMethod<'a> = Proto::mod_TxIn::OneOfsigning_method<'a>;

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
        Self::preimage_hashes_impl(proto).unwrap()
    }

    #[inline]
    pub fn compile(input: ()) -> Proto::SigningOutput<'static> {
        todo!()
    }

    fn preimage_hashes_impl(
        proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        // Convert Protobuf structure to `bitcoin` crate native transaction.
        // Prepare signing mechanism.
        let tx = convert_proto_to_tx(&proto).unwrap();
        let mut cache = SighashCache::new(&tx);

        let mut sighashes: Vec<Vec<u8>> = vec![];

        for (index, input) in proto.inputs.iter().enumerate() {
            match input.signing_method {
                // Use the legacy hashing mechanism (e.g. P2SH, P2PK, P2PKH).
                ProtoSigningMethod::legacy(ref legacy) => {
                    let script_pubkey = Script::from_bytes(legacy.script_pubkey.as_ref());
                    let sighash_type = if let SighashType::UseDefault = input.sighash {
                        EcdsaSighashType::All
                    } else {
                        EcdsaSighashType::from_consensus(input.sighash as u32)
                    };

                    let sighash = cache
                        .legacy_signature_hash(index, script_pubkey, sighash_type.to_u32())
                        .unwrap();

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

                    let sighash = cache
                        .segwit_signature_hash(
                            index,
                            script_pubkey.p2wpkh_script_code().as_ref().unwrap(),
                            value,
                            sighash_type,
                        )
                        .unwrap();

                    sighashes.push(sighash.as_byte_array().to_vec());
                },
                // Use the Taproot hashing mechanism (e.g. P2TR key-path/script-path)
                ProtoSigningMethod::taproot(ref taproot) => {
                    let leaf_hash = TapLeafHash::from_slice(taproot.leaf_hash.as_ref()).unwrap();
                    // Note that `input.sighash = 0` is handled by the underlying library.
                    let sighash_type =
                        TapSighashType::from_consensus_u8(input.sighash as u8).unwrap();

                    // This owner only exists to avoid running into lifetime
                    // issues related to `Prevouts::All(&[T])`.
                    let _owner;

                    let prevouts: Prevouts<'_, TxOut> = match taproot.prevout {
                        Proto::mod_TxIn::mod_Taproot::OneOfprevout::one(index) => {
                            let index = index as usize;

                            let txout = tx.output.get(index).unwrap();
                            Prevouts::One(
                                index,
                                TxOut {
                                    value: txout.value,
                                    // TODO: Can we avoid cloning here?
                                    script_pubkey: txout.script_pubkey.clone(),
                                },
                            )
                        },
                        Proto::mod_TxIn::mod_Taproot::OneOfprevout::all(_) => {
                            _owner = Some(
                                tx.output
                                    .iter()
                                    .map(|out| TxOut {
                                        value: out.value,
                                        // TODO: Can we avoid cloning here?
                                        script_pubkey: out.script_pubkey.clone(),
                                    })
                                    .collect::<Vec<TxOut>>(),
                            );

                            Prevouts::All(_owner.as_ref().unwrap())
                        },
                        _ => panic!(),
                    };

                    let sighash = cache
                        .taproot_signature_hash(
                            index,
                            &prevouts,
                            None,
                            Some((leaf_hash.into(), 0xFFFFFFFF)),
                            sighash_type,
                        )
                        .unwrap();

                    sighashes.push(sighash.as_byte_array().to_vec());
                },
                ProtoSigningMethod::None => panic!(),
            }
        }

        Ok(Proto::PreSigningOutput {
            sighashes: sighashes.into_iter().map(Cow::Owned).collect(),
        })
    }

    fn compile_impl(input: ()) -> Result<Proto::SigningOutput<'static>> {
        todo!()
    }
}

fn convert_proto_to_tx<'a>(proto: &Proto::SigningInput<'a>) -> Result<Transaction> {
    let version = proto.version;

    // Retreive the lock time. If none is provided, the default lock time is
    // used (immediately spendable).
    let lock_time = match proto.lock_time {
        ProtoLockTimeVariant::blocks(block) => {
            LockTime::Blocks(Height::from_consensus(block).unwrap())
        },
        ProtoLockTimeVariant::seconds(secs) => {
            LockTime::Seconds(Time::from_consensus(secs).unwrap())
        },
        ProtoLockTimeVariant::None => LockTime::Blocks(Height::from_consensus(0).unwrap()),
    };

    let mut tx = Transaction {
        version,
        lock_time,
        input: vec![],
        output: vec![],
    };

    for txin in &proto.inputs {
        let txid = Txid::from_slice(txin.txid.as_ref()).unwrap();
        let vout = txin.vout;

        tx.input.push(TxIn {
            previous_output: OutPoint { txid, vout },
            script_sig: ScriptBuf::new(),
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
