// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::evm_dispatcher;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Decode function call data to human readable json format, according to input abi json.
///
/// \param coin EVM-compatible coin type.
/// \param input The serialized data of `TW.EthereumAbi.Proto.ContractCallDecodingInput`.
/// \return serialized `EthereumAbi::Proto::ContractCallDecodingOutput`.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_abi_decode_contract_call(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), std::ptr::null_mut);

    evm_dispatcher
        .decode_abi_contract_call(input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Decode a function input or output data according to a given ABI.
///
/// \param coin EVM-compatible coin type.
/// \param input The serialized data of `TW.EthereumAbi.Proto.ParamsDecodingInput`.
/// \return The serialized data of a `TW.EthereumAbi.Proto.ParamsDecodingOutput` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_abi_decode_params(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), std::ptr::null_mut);

    evm_dispatcher
        .decode_abi_params(input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the function type signature, of the form "baz(int32,uint256)".
///
/// \param coin EVM-compatible coin type.
/// \param input The serialized data of `TW.EthereumAbi.Proto.FunctionGetTypeInput`.
/// \return function type signature as a Non-null string.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_abi_function_get_signature(
    coin: u32,
    input: *const TWData,
) -> *mut TWString {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), || TWString::new()
        .into_ptr());
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), || TWString::new().into_ptr());

    evm_dispatcher
        .get_abi_function_signature(input_data.as_slice())
        .map(|str| TWString::from(str).into_ptr())
        .unwrap_or_else(|_| TWString::new().into_ptr())
}

/// Encode function inputs to Eth ABI binary.
///
/// \param coin EVM-compatible coin type.
/// \param input The serialized data of `TW.EthereumAbi.Proto.FunctionEncodingInput`.
/// \return The serialized data of a `TW.EthereumAbi.Proto.FunctionEncodingOutput` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_abi_encode_function(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), std::ptr::null_mut);

    evm_dispatcher
        .encode_abi_function(input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// /// Decodes an Eth ABI value according to a given type.
///
/// \param coin EVM-compatible coin type.
/// \param input The serialized data of `TW.EthereumAbi.Proto.ValueDecodingInput`.
/// \return The serialized data of a `TW.EthereumAbi.Proto.ValueDecodingOutput` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_abi_decode_value(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), std::ptr::null_mut);

    evm_dispatcher
        .decode_abi_value(input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
