use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::nft::OrdinalNftInscription;
use crate::{
    Recipient, TXOutputP2TRScriptPath, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH,
};
use bitcoin::{PublicKey, WPubkeyHash};
use std::borrow::Cow;
use std::ffi::{c_char, CStr};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto::TransactionOutput;

#[no_mangle]
// Builds the P2PKH scriptPubkey.
pub unsafe extern "C" fn tw_build_p2pkh_script(
    satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let tx_out = TxOutputP2PKH::new(satoshis as u64, recipient);

    // Prepare and serialize protobuf structure.
    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");

    CByteArray::from(serialized)
}

#[no_mangle]
// Builds the P2WPKH scriptPubkey.
pub unsafe extern "C" fn tw_build_p2wpkh_script(
    satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(
        Recipient::<WPubkeyHash>::from_slice(slice),
        CByteArray::null
    );

    let tx_out = TxOutputP2WPKH::new(satoshis as u64, recipient);

    // Prepare and serialize protobuf structure.
    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");

    CByteArray::from(serialized)
}

#[no_mangle]
// Builds the P2TR key-path scriptPubkey.
pub unsafe extern "C" fn tw_build_p2tr_key_path_script(
    satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let tx_out = TxOutputP2TRKeyPath::new(satoshis as u64, recipient.into());

    // Prepare and serialize protobuf structure.
    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::default(),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");

    CByteArray::from(serialized)
}

#[no_mangle]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_build_brc20_transfer_inscription(
    // The 4-byte ticker.
    ticker: *const c_char,
    amount: u64,
    satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert ticket.
    let ticker = match CStr::from_ptr(ticker).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArray::null(),
    };

    if ticker.len() != 4 {
        return CByteArray::null();
    }

    let ticker = Ticker::new(ticker.to_string()).expect("ticker must be 4 bytes");

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    // Build transfer inscription.
    let transfer = BRC20TransferInscription::new(recipient, ticker, amount)
        .expect("transfer inscription implemented wrongly");

    let tx_out = TXOutputP2TRScriptPath::new(satoshis as u64, transfer.inscription().recipient());
    let spending_script = transfer.inscription().taproot_program();

    // Prepare and serialize protobuf structure.
    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::from(spending_script.as_bytes()),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");

    CByteArray::from(serialized)
}

#[no_mangle]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_bitcoin_build_nft_inscription(
    mime_type: *const c_char,
    data: *const u8,
    data_len: usize,
    satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert mimeType.
    let mime_type = match CStr::from_ptr(mime_type).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArray::null(),
    };

    // Convert data to inscribe.
    let data = try_or_else!(
        CByteArrayRef::new(data, data_len).as_slice(),
        CByteArray::null
    );

    // Convert Recipient.
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    // Inscribe NFT data.
    let nft = OrdinalNftInscription::new(mime_type.as_bytes(), data, recipient)
        .expect("Ordinal NFT inscription incorrectly constructed");

    let tx_out = TXOutputP2TRScriptPath::new(satoshis as u64, nft.inscription().recipient());
    let spending_script = nft.inscription().taproot_program();

    // Prepare and serialize protobuf structure.
    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::from(spending_script.as_bytes()),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");

    CByteArray::from(serialized)
}
