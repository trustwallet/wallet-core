use crate::input::TxInputP2TRKeyPath;
use crate::{output::TxOutputP2TRKeyPath, Recipient};
use bitcoin::{PublicKey, ScriptBuf, Txid};
use secp256k1::hashes::Hash;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct TWTxInputP2TRKeyPath(TxInputP2TRKeyPath);

impl RawPtrTrait for TWTxInputP2TRKeyPath {}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_input_p2tr_key_path_create(
    txid: *const u8,
    vout: u32,
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *const TWTxInputP2TRKeyPath {
    // Convert Txid.
    let slice = try_or_else!(CByteArrayRef::new(txid, 32).as_slice(), std::ptr::null);
    let txid = try_or_else!(Txid::from_slice(slice), std::ptr::null);

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        std::ptr::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), std::ptr::null);

    let input = TxInputP2TRKeyPath::builder()
        .txid(txid)
        .vout(vout)
        .recipient(recipient)
        .satoshis(satoshis)
        .build()
        // Never fails if all build methods are being called.
        .unwrap();

    TWTxInputP2TRKeyPath(input).into_ptr()
}
