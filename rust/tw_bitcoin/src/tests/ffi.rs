use crate::{ffi::taproot_build_and_sign_transaction, keypair_from_wif};
use crate::{TxInputP2PKH, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH};
use std::borrow::Cow;
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto::{
    SigningInput, TransactionPlan, TransactionVariant, UnspentTransaction,
};

#[test]
fn proto_sign_input_p2pkh_output_p2pkh() {
    use crate::tests::p2pkh::{
        ALICE_WIF, BOB_WIF, EXPECTED_RAW_SIGNED, FULL_AMOUNT, SEND_AMOUNT, TXID,
    };

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Note that the Txid must be reversed.
    let txid: Vec<u8> = hex::decode(TXID).unwrap().into_iter().rev().collect();

    // Prepare the scripts.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2PKH::only_script(bob.into());

    // Construct Protobuf payload.
    let input = SigningInput {
        // Ignored
        hash_type: 0,
        // Ignored
        amount: 0,
        // Ignored
        byte_fee: 0,
        // Ignored
        to_address: Cow::from(""),
        // Ignored
        change_address: Cow::from(""),
        private_key: vec![Cow::from(alice_privkey.as_slice())],
        scripts: Default::default(),
        utxo: vec![UnspentTransaction {
            out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                hash: Cow::from(txid.as_slice()),
                index: 0,
                // Ignored
                sequence: 0,
            }),
            script: Cow::from(in_script.as_bytes()),
            amount: FULL_AMOUNT as i64,
            variant: TransactionVariant::P2PKH,
        }],
        // Ignored
        use_max_amount: false,
        // Ignored
        coin_type: 0,
        plan: Some(TransactionPlan {
            // Ignored
            amount: 0,
            // Ignored
            available_amount: 0,
            // Ignored
            fee: 0,
            // Ignored
            change: 0,
            utxos: vec![UnspentTransaction {
                out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                    hash: Cow::from([].as_slice()),
                    index: 0,
                    sequence: 0,
                }),
                script: Cow::from(out_script.as_bytes()),
                amount: SEND_AMOUNT as i64,
                variant: TransactionVariant::P2PKH,
            }],
            // Ignored
            branch_id: Cow::from([].as_slice()),
            // Ignored
            error: tw_proto::Common::Proto::SigningError::OK,
            // Ignored
            output_op_return: Cow::from([].as_slice()),
        }),
        // Ignored
        lock_time: 0,
        // Ignored
        output_op_return: Cow::from([].as_slice()),
    };

    let sig = taproot_build_and_sign_transaction(input).unwrap();
    assert_eq!(hex::encode(&sig, false), EXPECTED_RAW_SIGNED);
}

#[test]
fn proto_sign_input_p2pkh_output_p2wpkh() {
    use crate::tests::p2wpkh::{
        ALICE_WIF, BOB_WIF, EXPECTED_RAW_SIGNED, FULL_AMOUNT, SEND_TO_BOB, TXID,
    };

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Note that the Txid must be reversed.
    let txid: Vec<u8> = hex::decode(TXID).unwrap().into_iter().rev().collect();

    // Prepare the scripts.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2WPKH::only_script(bob.try_into().unwrap());

    // Construct Protobuf payload.
    let input = SigningInput {
        // Ignored
        hash_type: 0,
        // Ignored
        amount: 0,
        // Ignored
        byte_fee: 0,
        // Ignored
        to_address: Cow::from(""),
        // Ignored
        change_address: Cow::from(""),
        private_key: vec![Cow::from(alice_privkey.as_slice())],
        scripts: Default::default(),
        utxo: vec![UnspentTransaction {
            out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                hash: Cow::from(txid.as_slice()),
                index: 0,
                // Ignored
                sequence: 0,
            }),
            script: Cow::from(in_script.as_bytes()),
            amount: FULL_AMOUNT as i64,
            variant: TransactionVariant::P2PKH,
        }],
        // Ignored
        use_max_amount: false,
        // Ignored
        coin_type: 0,
        plan: Some(TransactionPlan {
            // Ignored
            amount: 0,
            // Ignored
            available_amount: 0,
            // Ignored
            fee: 0,
            // Ignored
            change: 0,
            utxos: vec![UnspentTransaction {
                out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                    hash: Cow::from([].as_slice()),
                    index: 0,
                    sequence: 0,
                }),
                script: Cow::from(out_script.as_bytes()),
                amount: SEND_TO_BOB as i64,
                variant: TransactionVariant::P2WPKH,
            }],
            // Ignored
            branch_id: Cow::from([].as_slice()),
            // Ignored
            error: tw_proto::Common::Proto::SigningError::OK,
            // Ignored
            output_op_return: Cow::from([].as_slice()),
        }),
        // Ignored
        lock_time: 0,
        // Ignored
        output_op_return: Cow::from([].as_slice()),
    };

    let sig = taproot_build_and_sign_transaction(input).unwrap();
    assert_eq!(hex::encode(&sig, false), EXPECTED_RAW_SIGNED);
}

#[test]
fn proto_sign_input_p2pkh_output_p2tr_key_path() {
    use crate::tests::p2tr_key_path::{
        ALICE_WIF, BOB_WIF, EXPECTED_RAW_SIGNED, FULL_AMOUNT, SEND_TO_BOB, TXID,
    };

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    // Note that the Txid must be reversed.
    let txid: Vec<u8> = hex::decode(TXID).unwrap().into_iter().rev().collect();

    // Prepare the scripts.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2TRKeyPath::only_script(bob.try_into().unwrap());

    // Construct Protobuf payload.
    let input = SigningInput {
        // Ignored
        hash_type: 0,
        // Ignored
        amount: 0,
        // Ignored
        byte_fee: 0,
        // Ignored
        to_address: Cow::from(""),
        // Ignored
        change_address: Cow::from(""),
        private_key: vec![Cow::from(alice_privkey.as_slice())],
        scripts: Default::default(),
        utxo: vec![UnspentTransaction {
            out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                hash: Cow::from(txid.as_slice()),
                index: 0,
                // Ignored
                sequence: 0,
            }),
            script: Cow::from(in_script.as_bytes()),
            amount: FULL_AMOUNT as i64,
            variant: TransactionVariant::P2PKH,
        }],
        // Ignored
        use_max_amount: false,
        // Ignored
        coin_type: 0,
        plan: Some(TransactionPlan {
            // Ignored
            amount: 0,
            // Ignored
            available_amount: 0,
            // Ignored
            fee: 0,
            // Ignored
            change: 0,
            utxos: vec![UnspentTransaction {
                out_point: Some(tw_proto::Bitcoin::Proto::OutPoint {
                    hash: Cow::from([].as_slice()),
                    index: 0,
                    sequence: 0,
                }),
                script: Cow::from(out_script.as_bytes()),
                amount: SEND_TO_BOB as i64,
                variant: TransactionVariant::P2TRKEYPATH,
            }],
            // Ignored
            branch_id: Cow::from([].as_slice()),
            // Ignored
            error: tw_proto::Common::Proto::SigningError::OK,
            // Ignored
            output_op_return: Cow::from([].as_slice()),
        }),
        // Ignored
        lock_time: 0,
        // Ignored
        output_op_return: Cow::from([].as_slice()),
    };

    let sig = taproot_build_and_sign_transaction(input).unwrap();
    assert_eq!(hex::encode(&sig, false), EXPECTED_RAW_SIGNED);
}
