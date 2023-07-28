use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::hashes::Hash;
use bitcoin::sighash::{EcdsaSighashType, SighashCache};
use bitcoin::{
    secp256k1, OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness,
};
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::SigningResult;
use tw_proto::Utxo::Proto;

pub mod compiler;
pub mod entry;

type ProtoLockTimeVariant = Proto::mod_SigningInput::OneOflock_time;
type ProtoSpendingData<'a> = Proto::mod_TxIn::OneOfspending_data<'a>;
type ProtoSignerVariant = Proto::Signer;

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
        let private_key = secp256k1::SecretKey::from_slice(proto.private_key.as_ref()).unwrap();

        let tx = convert_proto_to_tx(&proto).unwrap();
        let mut cache = SighashCache::new(tx.clone());

        for (index, input) in tx.input.iter().enumerate() {
            let proto_input = proto.inputs.get(index).expect("index miscalculated");

            match proto_input.signer {
                ProtoSignerVariant::Legacy => {
                    let script_pubkey = Script::from_bytes(
                        proto_input.prevout.as_ref().unwrap().script_pubkey.as_ref(),
                    );
                    let sighash = proto_input.sighash as u32;

                    let hash = cache
                        .legacy_signature_hash(index, script_pubkey, sighash)
                        .unwrap();
                },
                Proto::Signer::Segwit => {
                    let script_pubkey = ScriptBuf::from_bytes(
                        proto_input.prevout.as_ref().unwrap().script_pubkey.to_vec(),
                    );
                    let sighash = EcdsaSighashType::from_consensus(proto_input.sighash as u32);
                    let value = proto_input.prevout.as_ref().unwrap().value;

                    let hash = cache
                        .segwit_signature_hash(
                            index,
                            script_pubkey.p2wpkh_script_code().as_ref().unwrap(),
                            value,
                            sighash,
                        )
                        .unwrap();
                },
                Proto::Signer::Taproot => {
                    todo!()
                },
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

        let (script_sig, witness) = match &txin.spending_data {
            ProtoSpendingData::script_sig(script) => {
                (ScriptBuf::from_bytes(script.to_vec()), Witness::new())
            },
            ProtoSpendingData::witness(witness) => {
                (ScriptBuf::new(), Witness::from_slice(&witness.items))
            },
            ProtoSpendingData::None => panic!(),
        };

        tx.input.push(TxIn {
            previous_output: OutPoint { txid, vout },
            script_sig,
            sequence: Sequence::default(),
            witness,
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
