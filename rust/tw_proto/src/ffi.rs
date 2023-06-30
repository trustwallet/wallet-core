// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use std::borrow::Cow;
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;

#[repr(C)]
pub enum CProtoError {
    Ok = 0,
    Internal = 1,
    ErrorDeserializingMsg = 2,
    ErrorSerializingMsg = 3,
}

impl From<CProtoError> for ErrorCode {
    fn from(e: CProtoError) -> Self {
        e as ErrorCode
    }
}

/// Takes a serialized `Ethereum::Proto::SigningInput` message, deserializes it and returns a serialized message back.
/// This FFI is used within integration tests.
/// \param input *non-null* byte array.
/// \param input_len length of the input byte array.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn pass_eth_signing_msg_through(
    input: *const u8,
    input_len: usize,
) -> CByteArrayResult {
    let data = CByteArrayRef::new(input, input_len)
        .to_vec()
        .unwrap_or_default();

    let msg: crate::Ethereum::Proto::SigningInput = match crate::deserialize(&data) {
        Ok(msg) => msg,
        Err(_) => return CByteArrayResult::error(CProtoError::ErrorDeserializingMsg),
    };
    crate::serialize(&msg)
        .map(CByteArray::from)
        .map_err(|_| CProtoError::ErrorSerializingMsg)
        .into()
}

/// Returns a serialized `Polkadot::Proto::SigningInput` message.
/// This FFI is used within integration tests.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn polkadot_test_signing_input() -> CByteArrayResult {
    use crate::Polkadot::Proto::{
        self as proto, mod_Balance::OneOfmessage_oneof as BalanceEnum,
        mod_SigningInput::OneOfmessage_oneof as SigningMsgEnum,
    };

    let block_hash = "0x343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642";
    let genesis_hash = "91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3";
    let to_address = "14E5nqKAp3oAJcmzgZhUD2RcptBeUBScxKHgJKU4HPNcKVf3";
    let privkey = [
        171, 248, 229, 189, 190, 48, 198, 86, 86, 192, 163, 203, 209, 129, 255, 138, 86, 41, 74,
        105, 223, 237, 210, 121, 130, 170, 206, 74, 118, 144, 145, 21,
    ];
    let value = [48, 57]; // 12345

    let block_hash = tw_encoding::hex::decode(block_hash).expect("Expect valid hash");
    let genesis_hash = tw_encoding::hex::decode(genesis_hash).expect("Expect valid hash");

    let balance = BalanceEnum::transfer(proto::mod_Balance::Transfer {
        to_address: Cow::from(to_address),
        value: Cow::Borrowed(&value),
        memo: Cow::default(),
        call_indices: None,
    });
    let signing_input = proto::SigningInput {
        block_hash: Cow::Owned(block_hash),
        genesis_hash: Cow::Owned(genesis_hash),
        nonce: 0,
        spec_version: 17,
        transaction_version: 3,
        era: Some(proto::Era {
            block_number: 927699,
            period: 8,
        }),
        private_key: Cow::Borrowed(&privkey),
        network: 0,
        message_oneof: SigningMsgEnum::balance_call(proto::Balance {
            message_oneof: balance,
        }),
        ..proto::SigningInput::default()
    };

    crate::serialize(&signing_input)
        .map(CByteArray::from)
        .map_err(|_| CProtoError::ErrorSerializingMsg)
        .into()
}
