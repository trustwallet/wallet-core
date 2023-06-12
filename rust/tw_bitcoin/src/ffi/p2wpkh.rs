#![allow(clippy::missing_safety_doc)]

use super::RawPtrTrait;
use crate::input::TxInputP2WPKH;
use crate::output::TxOutputP2WPKH;
use crate::{try_or_else, Recipient};
use bitcoin::{Txid, WPubkeyHash};
use secp256k1::hashes::Hash;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;

pub struct TWTxInputP2WPKH(pub(crate) TxInputP2WPKH);

impl RawPtrTrait for TWTxInputP2WPKH {}

pub struct TWTxOutputP2WPKH(pub(crate) TxOutputP2WPKH);

impl RawPtrTrait for TWTxOutputP2WPKH {}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_input_p2wpkh_create(
    txid: *const u8,
    vout: u32,
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *mut TWTxInputP2WPKH {
    // Convert Txid.
    let slice = try_or_else!(CByteArrayRef::new(txid, 32).as_slice(), std::ptr::null_mut);
    let txid = try_or_else!(Txid::from_slice(slice), std::ptr::null_mut);

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        std::ptr::null_mut
    );
    let recipient = try_or_else!(
        Recipient::<WPubkeyHash>::from_pubkey_slice(slice),
        std::ptr::null_mut
    );

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

#[no_mangle]
pub unsafe extern "C" fn tw_tx_output_p2wpkh_create(
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *mut TWTxOutputP2WPKH {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        std::ptr::null_mut
    );
    let recipient = try_or_else!(
        Recipient::<WPubkeyHash>::from_pubkey_slice(slice),
        std::ptr::null_mut
    );

    // Build P2WPKH scriptPubKey.
    let output = TxOutputP2WPKH::builder()
        .recipient(recipient)
        .satoshis(satoshis)
        .build()
        // Never fails if all build methods are being called.
        .unwrap();

    TWTxOutputP2WPKH(output).into_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_input_p2wpkh_delete(input: *mut TWTxInputP2WPKH) {
    let _ = TWTxInputP2WPKH::from_ptr(input);
}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_output_p2wpkh_delete(output: *mut TWTxOutputP2WPKH) {
    let _ = TWTxOutputP2WPKH::from_ptr(output);
}
