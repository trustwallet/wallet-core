use super::try_or_else;
use crate::{
    Error, Recipient, Result, TxInputP2PKH, TxInputP2TRKeyPath, TxInputP2WPKH, TxOutputP2PKH,
    TxOutputP2TRKeyPath, TxOutputP2WPKH,
};
use bitcoin::{PubkeyHash, PublicKey, ScriptBuf, Txid, WPubkeyHash};
use secp256k1::hashes::Hash;
use secp256k1::KeyPair;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_proto::Bitcoin::Proto::{SigningInput, TransactionVariant as TrVariant};

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2pkh_input_script(
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let script = TxInputP2PKH::only_script(recipient.into());
    CByteArray::from(script.to_bytes())
}

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2wpkh_input_script(
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

    let script = TxInputP2WPKH::only_script(recipient);
    CByteArray::from(script.to_bytes())
}

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2tr_key_path_input_script(
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let script = TxInputP2TRKeyPath::only_script(recipient.into());
    CByteArray::from(script.to_bytes())
}

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2pkh_output_script(
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let script = TxOutputP2PKH::only_script(recipient.into());
    CByteArray::from(script.to_bytes())
}

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2wpkh_output_script(
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

    let script = TxOutputP2WPKH::only_script(recipient);
    CByteArray::from(script.to_bytes())
}

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2tr_key_path_output_script(
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    let script = TxOutputP2TRKeyPath::only_script(recipient.into());
    CByteArray::from(script.to_bytes())
}
