use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::{
    Recipient, TXOutputP2TRScriptPath, TxOutputP2PKH, TxOutputP2TRKeyPath, TxOutputP2WPKH,
};
use bitcoin::{PublicKey, WPubkeyHash};
use std::borrow::Cow;
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
pub unsafe extern "C" fn tw_build_brc20_inscribe_transfer(
    // The 4-byte ticker.
    ticker: *const u8,
    amount: u64,
    satoshis: i64,
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
