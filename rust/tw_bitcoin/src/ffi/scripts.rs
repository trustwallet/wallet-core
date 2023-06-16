use super::try_or_else;
use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::{
    Recipient, TXOutputP2TRScriptPath, TaprootScript, TxOutputP2PKH, TxOutputP2TRKeyPath,
    TxOutputP2WPKH,
};
use bitcoin::{PublicKey, WPubkeyHash};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;

#[no_mangle]
// Builds the P2PKH scriptPubkey.
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
// Builds the P2WPKH scriptPubkey.
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
// Builds the P2TR key-path scriptPubkey.
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

#[no_mangle]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_build_brc20_inscribe_transfer(
    // The 4-byte ticker.
    ticker: *const u8,
    amount: u64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert ticket.
    let slice = try_or_else!(CByteArrayRef::new(ticker, 4).as_slice(), CByteArray::null);

    if slice.len() != 4 {
        return CByteArray::null();
    }

    let string = try_or_else!(String::from_utf8(slice.to_vec()), CByteArray::null);
    let ticker = Ticker::new(string).expect("ticker must be 4 bytes");

    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(Recipient::<PublicKey>::from_slice(slice), CByteArray::null);

    // Build transfer inscription.
    let transfer = BRC20TransferInscription::new(recipient, ticker, amount)
        .expect("transfer inscription implemented wrongly");

    let script = TXOutputP2TRScriptPath::only_script(transfer.0.recipient());
    CByteArray::from(script.to_bytes())
}
