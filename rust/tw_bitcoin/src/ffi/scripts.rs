use super::try_or_else;
use crate::{Recipient, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH};
use bitcoin::{PublicKey, WPubkeyHash};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;

#[no_mangle]
pub unsafe extern "C" fn tw_build_p2pkh_script(pubkey: *const u8, pubkey_len: usize) -> CByteArray {
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
pub unsafe extern "C" fn tw_build_p2wpkh_script(
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
pub unsafe extern "C" fn tw_build_p2tr_key_path_script(
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
