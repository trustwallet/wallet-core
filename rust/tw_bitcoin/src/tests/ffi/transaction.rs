use crate::ffi::taproot_build_and_sign_transaction;
use crate::tests::ffi::utils::{
    call_ffi_build_p2pkh_script, call_ffi_build_p2tr_key_path_script, call_ffi_build_p2wpkh_script,
    reverse_txid, ProtoSigningInputBuilder, ProtoTransactionBuilder,
};
use crate::{keypair_from_wif, Recipient};
use bitcoin::PublicKey;
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto::TransactionVariant;

#[test]
pub fn proto_sign_input_p2pkh_output_p2pkh() {
    use crate::tests::p2pkh::*;

    // Prepare keys.
    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from_keypair(&alice);

    let bob = keypair_from_wif(BOB_WIF).unwrap();
    let bob_recipient = Recipient::<PublicKey>::from_keypair(&bob);

    let txid = reverse_txid(TXID);

    // Prepare the scripts.
    let input = call_ffi_build_p2pkh_script(FULL_SATOSHIS, &alice_recipient);
    let output = call_ffi_build_p2pkh_script(SEND_SATOSHIS, &bob_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input.script)
                .satoshis(FULL_SATOSHIS)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output.script)
                .satoshis(SEND_SATOSHIS)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), TX_RAW);
}

#[test]
pub fn proto_sign_input_p2pkh_output_p2wpkh() {
    use crate::tests::p2wpkh::*;

    // Prepare keys.
    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from_keypair(&alice);

    let bob = keypair_from_wif(BOB_WIF).unwrap();
    let bob_recipient = Recipient::<PublicKey>::from_keypair(&bob);

    let txid = reverse_txid(TXID);

    // Prepare the scripts.
    let input = call_ffi_build_p2pkh_script(FULL_SATOSHIS, &alice_recipient);
    let output = call_ffi_build_p2wpkh_script(SEND_SATOSHIS, &bob_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input.script)
                .satoshis(FULL_SATOSHIS)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output.script)
                .satoshis(SEND_SATOSHIS)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), TX_RAW);
}

#[test]
pub fn proto_sign_input_p2pkh_output_p2tr_key_path() {
    use crate::tests::p2tr_key_path::*;

    // Prepare keys.
    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from_keypair(&alice);

    let bob = keypair_from_wif(BOB_WIF).unwrap();
    let bob_recipient = Recipient::<PublicKey>::from_keypair(&bob);

    let txid = reverse_txid(FIRST_TXID);

    // Prepare the scripts.
    let input = call_ffi_build_p2pkh_script(FULL_SATOSHIS, &alice_recipient);
    let output = call_ffi_build_p2tr_key_path_script(SEND_SATOSHIS_TO_BOB, &bob_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input.script)
                .satoshis(FULL_SATOSHIS)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output.script)
                .satoshis(SEND_SATOSHIS_TO_BOB)
                .variant(TransactionVariant::P2TRKEYPATH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), FIRST_TX_RAW);
}
