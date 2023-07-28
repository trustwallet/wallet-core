use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::hashes::Hash;
use bitcoin::{secp256k1, OutPoint, ScriptBuf, Sequence, Transaction, TxIn, Txid, Witness};
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::SigningResult;
use tw_proto::Utxo::Proto;

type ProtoLockTimeVariant = Proto::mod_SigningInput::OneOflock_time;
type ProtoSpendingData<'a> = Proto::mod_TxIn::OneOfspending_data<'a>;

pub mod compiler;
pub mod entry;

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
        let version = proto.version;

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

        for input in proto.inputs {
            let txin = input.txin.unwrap();
            let txid = Txid::from_slice(txin.txid.as_ref()).unwrap();
            let vout = txin.vout;

            let (script_sig, witness) = match txin.spending_data {
                ProtoSpendingData::script_isg(script) => {
                    (ScriptBuf::from_bytes(script.to_vec()), Witness::new())
                },
                ProtoSpendingData::witness(witness) => {
                    // TODO: Should be `repeated Witness...`
                    (ScriptBuf::new(), Witness::from_slice(&[witness.as_ref()]))
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

        todo!()
    }
}
