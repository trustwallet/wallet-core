use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::ffi::taproot_build_and_sign_transaction;
use crate::tests::ffi::utils::{
    call_ffi_build_brc20_transfer_script, call_ffi_build_p2wpkh_script, reverse_txid,
    ProtoSigningInputBuilder, ProtoTransactionBuilder,
};
use crate::tests::p2pkh::ALICE_WIF;
use crate::{keypair_from_wif, Recipient, TXOutputP2TRScriptPath};
use bitcoin::PublicKey;
use std::borrow::Cow;
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto::{TransactionOutput, TransactionVariant};

#[test]
fn proto_brc20_transfer_script() {
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let satoshis: u64 = 1_000;
    let brc20_amount = 20;
    let ticker = "oadf";

    // Call FFI function.
    let ffi_out = call_ffi_build_brc20_transfer_script(ticker, brc20_amount, satoshis, &recipient);

    // Compare with native call.
    let transfer = BRC20TransferInscription::new(
        recipient,
        Ticker::new(ticker.to_string()).unwrap(),
        brc20_amount,
    )
    .unwrap();

    let tapscript = transfer.0.recipient().clone();
    let spending_script = transfer.0.envelope.script;

    let tx_out = TXOutputP2TRScriptPath::new(satoshis, &tapscript);
    // Wrap in Protobuf structure.
    let proto = TransactionOutput {
        value: satoshis as i64,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::from(spending_script.as_bytes()),
    };

    assert_eq!(ffi_out, proto);
}

/// Commit the Inscription.
#[test]
fn proto_sign_brc20_transfer_inscription_commit() {
    use crate::tests::brc20_transfer::*;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from(&alice);
    let alice_pubkey = alice_recipient.public_key().to_bytes();

    // Note that the Txid must be reversed.
    let txid = reverse_txid(COMMIT_TXID);

    // Build input script.
    let input_p2wpkh = call_ffi_build_p2wpkh_script(FULL_AMOUNT, &alice_recipient);

    // Build inscription output.
    let output_inscribe = call_ffi_build_brc20_transfer_script(
        BRC20_TICKER,
        BRC20_AMOUNT,
        BRC20_INSCRIBE_AMOUNT,
        &alice_recipient,
    );

    // Build change output.
    let output_change = call_ffi_build_p2wpkh_script(FOR_FEE_AMOUNT, &alice_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(1)
                .script_pubkey(&input_p2wpkh.script)
                .satoshis(FULL_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_inscribe.script)
                .satoshis(BRC20_INSCRIBE_AMOUNT)
                .variant(TransactionVariant::BRC20TRANSFER)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_change.script)
                .satoshis(FOR_FEE_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), COMMIT_TX_RAW);
}

/// Reveal the Inscription.
#[test]
fn proto_sign_brc20_transfer_inscription_reveal() {
    use crate::tests::brc20_transfer::*;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from(&alice);

    // Note that the Txid must be reversed.
    let txid = reverse_txid(REVEAL_TXID);

    // Build input script.
    let input_inscription = call_ffi_build_brc20_transfer_script(
        BRC20_TICKER,
        BRC20_AMOUNT,
        BRC20_INSCRIBE_AMOUNT,
        &alice_recipient,
    );

    // Build inscription output.
    let output_p2wpkh = call_ffi_build_p2wpkh_script(BRC20_DUST_AMOUNT, &alice_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input_inscription.script)
                .satoshis(BRC20_INSCRIBE_AMOUNT)
                .variant(TransactionVariant::BRC20TRANSFER)
                // IMPORANT: include the witness containing the actual inscription.
                .spending_script(&input_inscription.spendingScript)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_p2wpkh.script)
                .satoshis(BRC20_DUST_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    let hex = hex::encode(&signed.encoded, false);

    assert_eq!(&hex[..164], REVEAL_RAW_P1);
    // We ignore the 64-byte Schnorr signature, since it uses random data for
    // signing on each construction and is therefore not reproducible.
    assert_ne!(&hex[164..292], REVEAL_RAW_SCHNORR);
    assert_eq!(&hex[292..], REVEAL_RAW_P2);
}

/// Transfer the Inscription with P2WPKH.
#[test]
fn proto_sign_brc20_transfer_inscription_p2wpkh_transfer() {
    use crate::tests::brc20_transfer::*;

    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let alice_privkey = alice.secret_bytes();
    let alice_recipient = Recipient::<PublicKey>::from(&alice);

    let bob = keypair_from_wif(BOB_WIF).unwrap();
    let bob_recipient = Recipient::<PublicKey>::from(&bob);

    // The Txid to reference the Inscription.
    // Note that the Txid must be reversed.
    let txid_inscription = reverse_txid(TRANSFER_TXID_INSCRIPTION);

    // The Txid for paying fees.
    let txid_for_fees = reverse_txid(TRANSFER_TXID_FOR_FEES);

    // Build input script for Inscription transfer.
    let input_transfer = call_ffi_build_p2wpkh_script(BRC20_DUST_AMOUNT, &alice_recipient);

    // Build input for paying fees.
    let input_fees = call_ffi_build_p2wpkh_script(FOR_FEE_AMOUNT, &alice_recipient);

    // Build Inscription transfer output with Bob as recipient.
    let output_transfer = call_ffi_build_p2wpkh_script(BRC20_DUST_AMOUNT, &bob_recipient);

    // Build change output.
    let output_change = call_ffi_build_p2wpkh_script(FOR_FEE_AMOUNT - MINER_FEE, &alice_recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&alice_privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid_inscription)
                .vout(0)
                .script_pubkey(&input_transfer.script)
                .satoshis(BRC20_DUST_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid_for_fees)
                .vout(1)
                .script_pubkey(&input_fees.script)
                .satoshis(FOR_FEE_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_transfer.script)
                .satoshis(BRC20_DUST_AMOUNT)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_change.script)
                .satoshis(FOR_FEE_AMOUNT - MINER_FEE)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), TRANSFER_RAW);
}
