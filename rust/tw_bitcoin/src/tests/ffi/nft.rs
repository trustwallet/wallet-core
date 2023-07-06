use crate::ffi::taproot_build_and_sign_transaction;
use crate::nft::{ImageType, NftInscription};
use crate::tests::ffi::utils::{
    call_ffi_build_brc20_transfer_script, call_ffi_build_p2wpkh_script, reverse_txid,
    ProtoSigningInputBuilder, ProtoTransactionBuilder,
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

    let mime_type = ImageType::ImagePng;
    let data = include_bytes!("../data/tw_logo.png");
    let satoshis: u64 = 1_000;

    // Call FFI function.
    let ffi_out = call_ffi_build_nft_inscription(mime_type, data, satoshis, &recipient);

    // Compare with native call.
    let nft = NftInscription::new(mime_type, data, recipient).unwrap();

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
