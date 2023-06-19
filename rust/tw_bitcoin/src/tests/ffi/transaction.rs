use crate::ffi::taproot_build_and_sign_transaction;
use crate::tests::ffi::{reverse_txid, ProtoSigningInputBuilder, ProtoTransactionBuilder};
use crate::{keypair_from_wif, TxInputP2PKH, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH};
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto::TransactionVariant;

#[test]
fn proto_sign_input_p2pkh_output_p2pkh() {
    use crate::tests::p2pkh::*;

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    let txid = reverse_txid(TXID);

    // Prepare the scripts.
    // Note that here the input and outputs script are identical (in_script =
    // out_script), the scriptSig/Witness for claiming a transaction is created
    // and set by the signer.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2PKH::only_script(bob.into());

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(in_script.as_bytes())
                .satoshis(FULL_AMOUNT)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(out_script.as_bytes())
                .satoshis(SEND_AMOUNT)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), EXPECTED_RAW_SIGNED);
}

#[test]
fn proto_sign_input_p2pkh_output_p2wpkh() {
    use crate::tests::p2wpkh::*;

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    let txid = reverse_txid(TXID);

    // Prepare the scripts.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2WPKH::only_script(bob.try_into().unwrap());

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(in_script.as_bytes())
                .satoshis(FULL_AMOUNT)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(out_script.as_bytes())
                .satoshis(SEND_TO_BOB)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), EXPECTED_RAW_SIGNED);
}

#[test]
fn proto_sign_input_p2pkh_output_p2tr_key_path() {
    use crate::tests::p2tr_key_path::*;

    let alice: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let bob = keypair_from_wif(BOB_WIF).unwrap();

    let txid = reverse_txid(TXID);

    // Prepare the scripts.
    let in_script = TxInputP2PKH::only_script(alice.into());
    let out_script = TxOutputP2TRKeyPath::only_script(bob.try_into().unwrap());

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(in_script.as_bytes())
                .satoshis(FULL_AMOUNT)
                .variant(TransactionVariant::P2PKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(out_script.as_bytes())
                .satoshis(SEND_TO_BOB)
                .variant(TransactionVariant::P2TRKEYPATH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), EXPECTED_RAW_SIGNED);
}
