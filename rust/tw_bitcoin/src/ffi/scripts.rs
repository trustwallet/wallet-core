use crate::brc20::Ticker;
use crate::entry::aliases::*;
use crate::nft::OrdinalNftInscription;
use crate::Recipient;
use bitcoin::{PublicKey, WPubkeyHash};
use std::ffi::{c_char, CStr};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::BitcoinV2::Proto;

#[no_mangle]
// Builds the P2PKH scriptPubkey.
pub unsafe extern "C" fn tw_build_p2pkh_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    //let tx_out = TxOutputP2PKH::new(satoshis as u64, recipient);
    let output = Proto::Output {
        amount: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::Builder {
            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                to_address: Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(
                    recipient.public_key().to_bytes().into(),
                ),
            }),
        }),
    };

    let res = try_or_else!(
        crate::modules::OutputBuilder::utxo_from_proto(&output),
        CByteArray::null
    );

    // Prepare and serialize protobuf structure.
    let proto = LegacyProto::TransactionOutput {
        value: res.value as i64,
        script: res.script_pubkey,
        spendingScript: Default::default(),
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");
    CByteArray::from(serialized)
}

#[no_mangle]
// Builds the P2WPKH scriptPubkey.
pub unsafe extern "C" fn tw_build_p2wpkh_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let _recipient = try_or_else!(
        Recipient::<WPubkeyHash>::from_slice(slice),
        CByteArray::null
    );

    todo!()
}

#[no_mangle]
// Builds the P2TR key-path scriptPubkey.
pub unsafe extern "C" fn tw_build_p2tr_key_path_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let _recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    todo!()
}

#[no_mangle]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_build_brc20_transfer_inscription(
    // The 4-byte ticker.
    ticker: *const c_char,
    _amount: u64,
    _satoshis: i64,
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

    let _ticker = Ticker::new(ticker.to_string()).expect("ticker must be 4 bytes");

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let _recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    // Prepare and serialize protobuf structure.
    /*
    let spending_script = transfer.inscription().taproot_program();

    let proto = TransactionOutput {
        value: satoshis,
        script: Cow::from(tx_out.script_pubkey.as_bytes()),
        spendingScript: Cow::from(spending_script.as_bytes()),
    };
    */

    todo!()
}

#[no_mangle]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_bitcoin_build_nft_inscription(
    mime_type: *const c_char,
    data: *const u8,
    data_len: usize,
    _satoshis: i64,
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
    let _nft = OrdinalNftInscription::new(mime_type.as_bytes(), data, recipient)
        .expect("Ordinal NFT inscription incorrectly constructed");

    todo!()
}
