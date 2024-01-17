// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::wallet_connect_request::WalletConnectRequest;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Parses the WalletConnect signing request as a `SigningInput`.
///
/// \param coin The given coin type to plan the transaction for.
/// \param input The serialized data of a `WalletConnect::Proto::ParseRequestInput` proto object.
/// \return The serialized data of `WalletConnect::Proto::ParseRequestOutput` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_wallet_connect_request_parse(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);

    WalletConnectRequest::parse(coin, input.as_slice())
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
