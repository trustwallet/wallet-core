use super::RawPtrTrait;
use crate::input::TxInputP2PKH;
use crate::output::TxOutputP2PKH;
use crate::{try_or_else, Recipient};
use bitcoin::{PublicKey, Txid};
use secp256k1::hashes::Hash;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;

pub struct TWTxInputP2PKH(pub(crate) TxInputP2PKH);

impl RawPtrTrait for TWTxInputP2PKH {}

pub struct TWTxOutputP2PKH(pub(crate) TxOutputP2PKH);

impl RawPtrTrait for TWTxOutputP2PKH {}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_input_p2pkh_create(
    txid: *const u8,
    vout: u32,
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *const TWTxInputP2PKH {
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
    let input = TxInputP2PKH::builder()
        .txid(txid)
        .vout(vout)
        .recipient(recipient)
        .satoshis(satoshis)
        .build()
        // Never fails if all build methods are being called.
        .unwrap();

    TWTxInputP2PKH(input).into_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn tw_tx_output_p2pkh_create(
    pubkey: *const u8,
    pubkey_len: usize,
    satoshis: u64,
) -> *const TWTxOutputP2PKH {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        std::ptr::null
    );
    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), std::ptr::null);

    // Build P2WPKH scriptPubKey.
    let output = TxOutputP2PKH::builder()
        .recipient(recipient)
        .satoshis(satoshis)
        .build()
        // Never fails if all build methods are being called.
        .unwrap();

    TWTxOutputP2PKH(output).into_ptr()
}
