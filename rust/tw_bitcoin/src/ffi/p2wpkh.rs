use crate::input::TxInputP2WPKH;
use crate::output::TxOutputP2WPKH;
use crate::Recipient;
use bitcoin::PublicKey;
use bitcoin::{ScriptBuf, Txid};
use secp256k1::hashes::Hash;
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct TWTxInputP2WPKH(TxInputP2WPKH);

impl RawPtrTrait for TWTxInputP2WPKH {}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_input_p2wpkh_create(
    txid: *const u8,
    vout: u32,
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *const TWTxInputP2WPKH {
    // Convert Txid.
    let slice = try_or_else!(CByteArrayRef::new(txid, 32).as_slice(), std::ptr::null);
    let txid = try_or_else!(Txid::from_slice(slice), std::ptr::null);

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        std::ptr::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), std::ptr::null);

    // Build P2WPKH scriptPubKey.
    let input = TxInputP2WPKH::builder()
        .txid(txid)
        .vout(vout)
        .recipient(recipient)
        .satoshis(satoshis)
        .build()
        // Never fails if all build methods are being called.
        .unwrap();

    TWTxInputP2WPKH(input).into_ptr()
}
