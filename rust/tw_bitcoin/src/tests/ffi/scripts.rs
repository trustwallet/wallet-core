use crate::ffi::tw_build_p2pkh_script;
use crate::tests::ffi::{ffi_build_p2tr_key_path_script, ffi_build_p2wpkh_script};
use crate::tests::p2pkh::ALICE_WIF;
use crate::{keypair_from_wif, Recipient, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH};
use bitcoin::PublicKey;
use std::borrow::Cow;
use tw_proto::Bitcoin::Proto::TransactionOutput;

#[test]
fn build_ffi_p2pkh_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let alice_pubkey = recipient.public_key().to_bytes();
    let array = unsafe {
        tw_build_p2pkh_script(satoshis as i64, alice_pubkey.as_ptr(), alice_pubkey.len()).into_vec()
    };

    let ffi_der: TransactionOutput = tw_proto::deserialize(&array).unwrap();

    // Compare with native call.
    let tx_out = TxOutputP2PKH::new(satoshis, recipient);
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(ffi_der, proto);
}

#[test]
fn build_ffi_p2wpkh_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let transaction = ffi_build_p2wpkh_script(satoshis, &recipient);

    // Compare with native call.
    let tx_out = TxOutputP2WPKH::new(satoshis, recipient.try_into().unwrap());
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(transaction, proto);
}

#[test]
fn build_ffi_p2tr_key_path_script() {
    // Prepare keys.
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;

    // Call FFI function.
    let transaction = ffi_build_p2tr_key_path_script(satoshis, &recipient);

    // Compare with native call.
    let tx_out = TxOutputP2TRKeyPath::new(satoshis, recipient.try_into().unwrap());
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    assert_eq!(transaction, proto);
}
