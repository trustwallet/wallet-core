use crate::ffi::taproot_build_and_sign_transaction;
use crate::nft::OrdinalNftInscription;
use crate::tests::ffi::utils::{
    call_ffi_build_p2wpkh_script, reverse_txid, ProtoSigningInputBuilder, ProtoTransactionBuilder,
};
use crate::tests::nft::ALICE_WIF;
use crate::{keypair_from_wif, Recipient, TXOutputP2TRScriptPath};
use bitcoin::PublicKey;
use std::borrow::Cow;
use tw_encoding::hex;
use tw_proto::Bitcoin::Proto::{TransactionOutput, TransactionVariant};

use super::utils::call_ffi_build_nft_inscription;

#[test]
fn proto_nft_inscription_script() {
    let keypair: secp256k1::KeyPair = keypair_from_wif(ALICE_WIF).unwrap();
    let recipient = Recipient::<PublicKey>::from(keypair);

    let mime_type = b"image/png";
    let payload = hex::decode(crate::tests::data::NFT_INSCRIPTION_IMAGE_DATA).unwrap();
    let satoshis: u64 = 1_000;

    // Call FFI function.
    let ffi_out = call_ffi_build_nft_inscription(mime_type, &payload, satoshis, &recipient);

    // Compare with native call.
    let nft = OrdinalNftInscription::new(mime_type, &payload, recipient).unwrap();

    let tapscript = nft.inscription().recipient().clone();
    let spending_script = nft.inscription().taproot_program();

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
fn proto_sign_nft_inscription_commit() {
    use crate::tests::nft::*;

    // Prepare keys.
    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let privkey = alice.secret_bytes();
    let recipient = Recipient::<PublicKey>::from(&alice);

    let mime_type = b"image/png";
    let payload = hex::decode(crate::tests::data::NFT_INSCRIPTION_IMAGE_DATA).unwrap();
    let satoshis: u64 = 1_000;

    // Note that the Txid must be reversed.
    let txid = reverse_txid(COMMIT_TXID);

    // Build input script.
    let input = call_ffi_build_p2wpkh_script(FULL_SATOSHIS, &recipient);

    // Build inscription output.
    let output = call_ffi_build_nft_inscription(mime_type, &payload, satoshis, &recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input.script)
                .satoshis(FULL_SATOSHIS)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output.script)
                .satoshis(INSCRIBE_SATOSHIS)
                .variant(TransactionVariant::NFTINSCRIPTION)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    assert_eq!(hex::encode(&signed.encoded, false), COMMIT_RAW_TX);
}

/// Reveal the Inscription.
#[test]
fn proto_sign_nft_inscription_reveal() {
    use crate::tests::nft::*;

    // Prepare keys.
    let alice = keypair_from_wif(ALICE_WIF).unwrap();
    let privkey = alice.secret_bytes();
    let recipient = Recipient::<PublicKey>::from(&alice);

    let mime_type = b"image/png";
    let payload = hex::decode(crate::tests::data::NFT_INSCRIPTION_IMAGE_DATA).unwrap();
    let satoshis: u64 = 1_000;

    // Note that the Txid must be reversed.
    let txid = reverse_txid(REVEAL_TXID);

    // Build inscription input.
    let input = call_ffi_build_nft_inscription(mime_type, &payload, satoshis, &recipient);

    // Build inscription output.
    let output_p2wpkh = call_ffi_build_p2wpkh_script(DUST_SATOSHIS, &recipient);

    // Construct Protobuf payload.
    let signing = ProtoSigningInputBuilder::new()
        .private_key(&privkey)
        .input(
            ProtoTransactionBuilder::new()
                .txid(&txid)
                .vout(0)
                .script_pubkey(&input.script)
                .satoshis(INSCRIBE_SATOSHIS)
                .variant(TransactionVariant::NFTINSCRIPTION)
                // IMPORANT: include the witness containing the actual inscription.
                .spending_script(&input.spendingScript)
                .build(),
        )
        .output(
            ProtoTransactionBuilder::new()
                .script_pubkey(&output_p2wpkh.script)
                .satoshis(DUST_SATOSHIS)
                .variant(TransactionVariant::P2WPKH)
                .build(),
        )
        .build();

    let signed = taproot_build_and_sign_transaction(signing).unwrap();
    let hex = hex::encode(&signed.encoded, false);

    assert_eq!(hex[..164], REVEAL_RAW_TX[..164]);
    // We ignore the 64-byte Schnorr signature, since it uses random data for
    // signing on each construction and is therefore not reproducible.
    assert_ne!(hex[164..292], REVEAL_RAW_TX[164..292]);
    assert_eq!(hex[292..], REVEAL_RAW_TX[292..]);
}
