use bitcoin::hashes::Hash;
use tw_coin_entry::coin_entry::{CoinEntry, CoinAddress, SignatureBytes, PublicKeyBytes};
use tw_coin_entry::error::SigningResult;
use tw_proto::Polkadot::Proto::mod_CallIndices::OneOfvariant;
use tw_proto::Utxo::Proto::mod_TxIn::OneOfspending_data;
use std::marker::PhantomData;
use bitcoin::{secp256k1, Transaction, TxIn, ScriptBuf, Witness, Sequence, OutPoint, Txid};
use bitcoin::blockdata::locktime::absolute::{Height, Time, LockTime};

pub mod entry;
pub mod compiler;

use tw_proto::Utxo::Proto;

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
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningInput<'static>> {
		let private_key = secp256k1::SecretKey::from_slice(input.private_key.as_ref()).unwrap();

		let lock_time = match input.lock_time {
			Proto::mod_SigningInput::OneOflock_time::blocks(block) => {
				LockTime::Blocks(Height::from_consensus(block).unwrap())
			}
			Proto::mod_SigningInput::OneOflock_time::seconds(secs) => {
				LockTime::Seconds(Time::from_consensus(secs).unwrap())
			}
			Proto::mod_SigningInput::OneOflock_time::None => {
				LockTime::Blocks(Height::from_consensus(0).unwrap())
			}
		};

		let mut tx = Transaction {
			version: input.version,
			lock_time,
			input: vec![],
			output: vec![],
		};

		for input in input.inputs {
			let txin = match input.variant {
				Proto::mod_TxVariant::OneOfvariant::legacy_input(legacy) => {
					legacy.txin.unwrap()
				}
				_ => panic!(),
			};

			let (script_sig, witness) = match txin.spending_data {
				Proto::mod_TxIn::OneOfspending_data::script_isg(script) => {
					(ScriptBuf::from_bytes(script.to_vec()), Witness::new())
				}
				Proto::mod_TxIn::OneOfspending_data::witness(witness) => {
					// TODO: Should be `repeated Witness...` 
					(ScriptBuf::new(), Witness::from_slice(&[witness.as_ref()]))
				}
				Proto::mod_TxIn::OneOfspending_data::None => panic!(),
			};

			tx.input.push(TxIn {
				previous_output: OutPoint {
					txid: Txid::from_slice(txin.txid.as_ref()).unwrap(),
					vout: txin.vout,
				},
				script_sig,
				sequence: Sequence::default(),
				witness,
			});
		}

		todo!()
	}
}
