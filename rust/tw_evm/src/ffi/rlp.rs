// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::evm_context::StandardEvmContext;
use crate::modules::rlp_encoder::RlpEncoder;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_proto::{deserialize, serialize};

/// Encodes an item or a list of items as Eth RLP binary format.
///
/// \param input Non-null serialized `EthereumRlp::Proto::EncodingInput`.
/// \return serialized `EthereumRlp::Proto::EncodingOutput`.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_rlp_encode(input: *const TWData) -> *mut TWData {
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let input = try_or_else!(deserialize(input_data.as_slice()), std::ptr::null_mut);
    let output = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
    let output_data = try_or_else!(serialize(&output), std::ptr::null_mut);
    TWData::from(output_data).into_ptr()
}

// /// Encodes the given U256 `number` to Eth RLP binary.
// ///
// /// \param number Non-null U256 in big-endian binary representation.
// /// \return Non-null RLP binary representation.
// #[no_mangle]
// pub unsafe extern "C" fn tw_ethereum_rlp_encode_bytes(bytes: *const TWData) -> *mut TWData {
//     let bytes = try_or_else!(TWData::from_ptr_as_ref(item), std::ptr::null_mut);
//     RlpEncoder::encode_bytes(number.as_slice())
//         .map(|encoded| TWData::from(encoded).into_ptr())
//         .unwrap_or_else(|_| std::ptr::null_mut())
// }
//
// /// Encodes the given U256 `number` to Eth RLP binary.
// ///
// /// \param number Non-null U256 in big-endian binary representation.
// /// \return Non-null RLP binary representation.
// #[no_mangle]
// pub unsafe extern "C" fn tw_ethereum_rlp_encode_list(number: *const TWData) -> *mut TWData {
//     let number = try_or_else!(TWData::from_ptr_as_ref(item), std::ptr::null_mut);
//     RlpEncoder::encode_u256_be(number.as_slice())
//         .map(|encoded| TWData::from(encoded).into_ptr())
//         .unwrap_or_else(|_| std::ptr::null_mut())
// }
