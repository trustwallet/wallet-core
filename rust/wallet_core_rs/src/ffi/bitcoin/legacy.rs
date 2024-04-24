// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::borrow::Cow;
use std::ffi::{c_char, CStr};
use tw_bitcoin::aliases::*;
use tw_bitcoin::native::consensus::Decodable;
use tw_bitcoin::native::{PublicKey, Transaction};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::CUInt64Result;
use tw_misc::try_or_else;
use tw_proto::Bitcoin::Proto as LegacyProto;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto as CommonProto;

// NOTE: The tests for those APIs can be found in `tw_bitcoin`.

#[no_mangle]
#[deprecated]
// Builds the P2PKH scriptPubkey.
pub unsafe extern "C" fn tw_bitcoin_legacy_build_p2pkh_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(PublicKey::from_slice(slice), CByteArray::null);

    let output = Proto::Output {
        value: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(recipient.to_bytes().into()),
            }),
        }),
    };

    let res = try_or_else!(
        tw_bitcoin::modules::transactions::OutputBuilder::utxo_from_proto(&output),
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
#[deprecated]
// Builds the P2WPKH scriptPubkey.
pub unsafe extern "C" fn tw_bitcoin_legacy_build_p2wpkh_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(PublicKey::from_slice(slice), CByteArray::null);

    let output = Proto::Output {
        value: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                to_address: ProtoPubkeyOrHash::pubkey(recipient.to_bytes().into()),
            }),
        }),
    };

    let res = try_or_else!(
        tw_bitcoin::modules::transactions::OutputBuilder::utxo_from_proto(&output),
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
#[deprecated]
// Builds the P2TR key-path scriptPubkey.
pub unsafe extern "C" fn tw_bitcoin_legacy_build_p2tr_key_path_script(
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );
    let recipient = try_or_else!(PublicKey::from_slice(slice), CByteArray::null);

    let output = Proto::Output {
        value: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::p2tr_key_path(recipient.to_bytes().into()),
        }),
    };

    let res = try_or_else!(
        tw_bitcoin::modules::transactions::OutputBuilder::utxo_from_proto(&output),
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
#[deprecated]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_bitcoin_legacy_build_brc20_transfer_inscription(
    // The 4-byte ticker.
    ticker: *const c_char,
    amount: *const c_char,
    _satoshis: i64,
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArray {
    // Convert Recipient
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(PublicKey::from_slice(slice), CByteArray::null);

    // Convert ticket.
    let ticker = match CStr::from_ptr(ticker).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArray::null(),
    };

    let amount = match CStr::from_ptr(amount).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArray::null(),
    };

    let output = Proto::Output {
        value: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::brc20_inscribe(
                Proto::mod_Output::OutputBrc20Inscription {
                    inscribe_to: recipient.to_bytes().into(),
                    ticker: ticker.into(),
                    transfer_amount: Cow::from(amount.to_string()),
                },
            ),
        }),
    };

    let res = try_or_else!(
        tw_bitcoin::modules::transactions::OutputBuilder::utxo_from_proto(&output),
        CByteArray::null
    );

    // Prepare and serialize protobuf structure.
    let proto = LegacyProto::TransactionOutput {
        value: res.value as i64,
        script: res.script_pubkey,
        spendingScript: res.taproot_payload,
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");
    CByteArray::from(serialized)
}

#[no_mangle]
#[deprecated]
// Builds the Ordinals inscripton for BRC20 transfer.
pub unsafe extern "C" fn tw_bitcoin_legacy_build_nft_inscription(
    mime_type: *const c_char,
    payload: *const u8,
    payload_len: usize,
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
    let payload = try_or_else!(
        CByteArrayRef::new(payload, payload_len).as_slice(),
        CByteArray::null
    );

    // Convert Recipient.
    let slice = try_or_else!(
        CByteArrayRef::new(pubkey, pubkey_len).as_slice(),
        CByteArray::null
    );

    let recipient = try_or_else!(PublicKey::from_slice(slice), CByteArray::null);

    // Inscribe NFT data.
    let output = Proto::Output {
        value: _satoshis as u64,
        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
            variant: ProtoOutputBuilder::ordinal_inscribe(
                Proto::mod_Output::OutputOrdinalInscription {
                    inscribe_to: recipient.to_bytes().into(),
                    mime_type: mime_type.into(),
                    payload: payload.into(),
                },
            ),
        }),
    };

    let res = try_or_else!(
        tw_bitcoin::modules::transactions::OutputBuilder::utxo_from_proto(&output),
        CByteArray::null
    );

    // Prepare and serialize protobuf structure.
    let proto = LegacyProto::TransactionOutput {
        value: res.value as i64,
        script: res.script_pubkey,
        spendingScript: res.taproot_payload,
    };

    let serialized = tw_proto::serialize(&proto).expect("failed to serialized transaction output");
    CByteArray::from(serialized)
}

#[deprecated]
#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_legacy_calculate_transaction_fee(
    input: *const u8,
    input_len: usize,
    sat_vb: u64,
) -> CUInt64Result {
    let Some(mut encoded) = CByteArrayRef::new(input, input_len).as_slice() else {
        return CUInt64Result::error(1);
    };

    // Decode transaction.
    let Ok(tx) = Transaction::consensus_decode(&mut encoded) else {
        return CUInt64Result::error(1);
    };

    // Calculate fee.
    let weight = tx.weight();
    let fee = weight.to_vbytes_ceil() * sat_vb;

    CUInt64Result::ok(fee)
}

#[no_mangle]
#[deprecated]
pub unsafe extern "C" fn tw_bitcoin_legacy_taproot_build_and_sign_transaction(
    input: *const u8,
    input_len: usize,
) -> CByteArray {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let proto: LegacyProto::SigningInput =
        try_or_else!(tw_proto::deserialize(&data), CByteArray::null);

    let Ok(signing) = tw_bitcoin::modules::legacy::taproot_build_and_sign_transaction(proto) else {
        // Convert the `BitcoinV2.proto` error type inot the `Common.proto`
        // errot type and return.
        let error = LegacyProto::SigningOutput {
            error: CommonProto::SigningError::Error_general,
            ..Default::default()
        };

        let serialized = tw_proto::serialize(&error).expect("failed to serialize error message");
        return CByteArray::from(serialized);
    };

    // Serialize SigningOutput and return.
    let serialized = tw_proto::serialize(&signing).expect("failed to serialize signed transaction");
    CByteArray::from(serialized)
}
