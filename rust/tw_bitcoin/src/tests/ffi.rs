use super::ONE_BTC;
use crate::{ffi::taproot_build_and_sign_transaction, keypair_from_wif};
use crate::tests::p2pkh::{ALICE_WIF, BOB_WIF, TXID, FULL_AMOUNT, SEND_AMOUNT};
use tw_proto::Bitcoin::Proto::{SigningInput, TransactionPlan, UnspentTransaction, TransactionVariant};
use tw_encoding::hex;
use std::borrow::Cow;

#[test]
fn proto_sign_input_p2pkh_output_p2pkh() {
	let alice = keypair_from_wif(ALICE_WIF).unwrap();

    let input = SigningInput {
        hash_type: 0,
		amount: (10 * ONE_BTC) as i64,
		byte_fee: 0,
		to_address: Cow::from(""),
		change_address: Cow::from(""),
		private_key: vec![Cow::from(alice.secret_bytes().as_slice())],
		scripts: Default::default(),
		utxo: vec![
			UnspentTransaction {
				out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
					hash: Cow::from(TXID.as_bytes()),
					index: 0,
					sequence: 0,
				}),
				script: Cow::from([].as_slice()),
				amount: FULL_AMOUNT as i64,
				variant: TransactionVariant::P2PKH,
			}
		],
		use_max_amount: false,
		coin_type: 0,
		plan: Some(TransactionPlan {
			amount: 0,
			available_amount: 0,
			fee: 0,
			change: 0,
			utxos: vec![
				UnspentTransaction {
					out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
						hash: Cow::from([].as_slice()),
						index: 0,
						sequence: 0,
					}),
					script: Cow::from([].as_slice()),
					amount: SEND_AMOUNT as i64,
					variant: TransactionVariant::P2PKH,
				}
			],
			branch_id: Cow::from([].as_slice()),
			error: tw_proto::Common::Proto::SigningError::OK,
			output_op_return: Cow::from([].as_slice()),
		}),
		lock_time: 0,
		output_op_return: Cow::from([].as_slice())

    };

    const EXPECTED_RAW_SIGNED: &str = "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000";

    let sig = taproot_build_and_sign_transaction(input).unwrap();
	assert_eq!(hex::encode(&sig, false), EXPECTED_RAW_SIGNED);
}
