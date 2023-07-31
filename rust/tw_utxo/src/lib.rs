use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::hashes::Hash;
use bitcoin::secp256k1::KeyPair;
use bitcoin::sighash::{EcdsaSighashType, Prevouts, SighashCache, TapSighashType};
use bitcoin::taproot::{LeafVersion, TapLeafHash};
use bitcoin::{
    secp256k1, OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness,
};
use secp256k1::Secp256k1;
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::SigningResult;
use tw_proto::Utxo::Proto;

pub mod compiler;
pub mod entry;

type ProtoLockTimeVariant = Proto::mod_SigningInput::OneOflock_time;
type ProtoSignerVariant<'a> = Proto::mod_TxIn::OneOfsigner<'a>;
type ProtoPrevoutVariant<'a> = Proto::mod_TxIn::mod_Taproot::OneOfprevouts<'a>;

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

pub struct Signer<Context: UtxoContext> {
    _phantom: PhantomData<Context>,
}

impl Signer<StandardBitcoinContext> {
    #[inline]
    pub fn sign_proto(input: Proto::SigningInput<'_>) -> Proto::SigningInput {
        Self::sign_proto_impl(input).unwrap();
        todo!()
    }

    fn sign_proto_impl(
        proto: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningInput<'static>> {
        let secp = Secp256k1::new();
        let keypair = KeyPair::from_seckey_slice(&secp, proto.private_key.as_ref()).unwrap();

        // Convert Protobuf structure to `bitcoin` crate native transaction.
        // Prepare signing mechanism.
        let tx = convert_proto_to_tx(&proto).unwrap();
        let mut cache = SighashCache::new(&tx);

        for (index, input) in proto.inputs.iter().enumerate() {
            match input.signer {
                // Use the legacy hashing mechanism (e.g. P2SH, P2PK, P2PKH).
                ProtoSignerVariant::legacy(ref legacy) => {
                    let script_pubkey = Script::from_bytes(legacy.script_pubkey.as_ref());
                    let sighash = input.sighash as u32;

                    let hash = cache
                        .legacy_signature_hash(index, script_pubkey, sighash)
                        .unwrap();

                    let sighash = secp256k1::Message::from_slice(hash.as_byte_array()).unwrap();
                    let sig = secp.sign_ecdsa(&sighash, &keypair.secret_key());
                },
                // Use the Segwit hashing mechanism (e.g. P2WSH, P2WPKH).
                ProtoSignerVariant::segwit(ref segwit) => {
                    let script_pubkey = ScriptBuf::from_bytes(segwit.script_pubkey.to_vec());
                    let sighash = EcdsaSighashType::from_consensus(input.sighash as u32);
                    let value = segwit.value;

                    let hash = cache
                        .segwit_signature_hash(
                            index,
                            script_pubkey.p2wpkh_script_code().as_ref().unwrap(),
                            value,
                            sighash,
                        )
                        .unwrap();

                    let sighash = secp256k1::Message::from_slice(hash.as_byte_array()).unwrap();
                    let sig = secp.sign_ecdsa(&sighash, &keypair.secret_key());
                },
                // Use the Taproot hashing mechanism (e.g. P2TR key-path/script-path)
                ProtoSignerVariant::taproot(ref taproot) => {
                    let leaf_hash = TapLeafHash::from_slice(taproot.leaf_hash.as_ref()).unwrap();
                    let sighash = TapSighashType::from_consensus_u8(input.sighash as u8).unwrap();

                    let mut owner = None;
                    let prevouts = match taproot.prevouts {
                        ProtoPrevoutVariant::one(ref one) => {
                            let script_pubkey = ScriptBuf::from_bytes(
                                one.txout.as_ref().unwrap().script_pubkey.to_vec(),
                            );

                            Prevouts::One(
                                one.index as usize,
                                TxOut {
                                    value: one.txout.as_ref().unwrap().value,
                                    script_pubkey,
                                },
                            )
                        },
                        ProtoPrevoutVariant::all(ref all) => {
                            owner = Some(
                                all.items
                                    .iter()
                                    .map(|out| TxOut {
                                        value: out.value,
                                        script_pubkey: ScriptBuf::from_bytes(
                                            out.script_pubkey.to_vec(),
                                        ),
                                    })
                                    .collect::<Vec<TxOut>>(),
                            );

                            Prevouts::All(owner.as_ref().unwrap())
                        },
                        ProtoPrevoutVariant::None => panic!(),
                    };

                    let hash = cache
                        .taproot_signature_hash(
                            index,
                            &prevouts,
                            None,
                            Some((leaf_hash.into(), 0xFFFFFFFF)),
                            sighash,
                        )
                        .unwrap();

                    let sighash = secp256k1::Message::from_slice(hash.as_byte_array()).unwrap();
                    let sig = secp.sign_schnorr(&sighash, &keypair);
                },
                ProtoSignerVariant::None => panic!(),
            }
        }

        todo!()
    }
}

fn convert_proto_to_tx<'a>(proto: &Proto::SigningInput<'a>) -> Result<Transaction, ()> {
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
