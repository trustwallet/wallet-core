use crate::tests::ffi::utils::{
    call_ffi_build_p2pkh_script, call_ffi_build_p2tr_key_path_script, call_ffi_build_p2wpkh_script,
};
use crate::tests::p2pkh::ALICE_WIF;
use crate::{keypair_from_wif, Recipient, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH};
use bitcoin::PublicKey;
use std::borrow::Cow;
use tw_proto::Bitcoin::Proto::TransactionOutput;

#[test]
fn proto_build_p2pkh_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let ffi_out = call_ffi_build_p2pkh_script(satoshis, &recipient);

    // Compare with native call.
    let tx_out = TxOutputP2PKH::new(satoshis, recipient);
    // Wrap in Protobuf structure.
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(ffi_out, proto);
}

#[test]
fn proto_build_p2wpkh_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let ffi_out = call_ffi_build_p2wpkh_script(satoshis, &recipient);

    // Compare with native call.
    let tx_out = TxOutputP2WPKH::new(satoshis, recipient.try_into().unwrap());
    // Wrap in Protobuf structure.
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(ffi_out, proto);
}

#[test]
fn proto_build_p2tr_key_path_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let ffi_out = call_ffi_build_p2tr_key_path_script(satoshis, &recipient);

    // Compare with native call.
    let tx_out = TxOutputP2TRKeyPath::new(satoshis, recipient.try_into().unwrap());
    // Wrap in Protobuf structure.
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(ffi_out, proto);
}
