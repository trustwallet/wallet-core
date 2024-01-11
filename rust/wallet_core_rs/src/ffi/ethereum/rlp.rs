// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::evm_dispatcher;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Encodes an item or a list of items as Eth RLP binary format.
///
/// \param coin EVM-compatible coin type.
/// \param input Non-null serialized `EthereumRlp::Proto::EncodingInput`.
/// \return serialized `EthereumRlp::Proto::EncodingOutput`.
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_rlp_encode(coin: u32, input: *const TWData) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let evm_dispatcher = try_or_else!(evm_dispatcher(coin), std::ptr::null_mut);
    evm_dispatcher
        .encode_rlp(input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
