// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::str::FromStr;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_ton::address::TonAddress;
use tw_ton::modules::address_converter::AddressConverter;

/// Converts a TON user address into a Bag of Cells (BoC) with a single root Cell.
/// The function is mostly used to request a Jetton user address via `get_wallet_address` RPC.
/// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
///
/// \param address Address to be converted into a Bag Of Cells (BoC).
/// \return Pointer to a base64 encoded Bag Of Cells (BoC). Null if invalid address provided.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_address_converter_to_boc(
    address: *const TWString,
) -> *mut TWString {
    let address = try_or_else!(TWString::from_ptr_as_ref(address), std::ptr::null_mut);
    let address_str = try_or_else!(address.as_str(), std::ptr::null_mut);
    let address_ton = try_or_else!(TonAddress::from_str(address_str), std::ptr::null_mut);

    let boc_encoded = try_or_else!(
        AddressConverter::convert_to_boc_base64(&address_ton),
        std::ptr::null_mut
    );

    TWString::from(boc_encoded).into_ptr()
}

/// Parses a TON address from a Bag of Cells (BoC) with a single root Cell.
/// The function is mostly used to parse a Jetton user address received on `get_wallet_address` RPC.
/// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
///
/// \param boc Base64 encoded Bag Of Cells (BoC).
/// \return Pointer to a Jetton address.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_address_converter_from_boc(boc: *const TWString) -> *mut TWString {
    let boc = try_or_else!(TWString::from_ptr_as_ref(boc), std::ptr::null_mut);
    let boc_str = try_or_else!(boc.as_str(), std::ptr::null_mut);

    let address_ton = try_or_else!(
        AddressConverter::parse_from_boc_base64(boc_str),
        std::ptr::null_mut
    );

    TWString::from(address_ton.to_string()).into_ptr()
}

/// Converts any TON address format to user friendly with the given parameters.
///
/// \param address raw or user-friendly address to be converted.
/// \param bounceable whether the result address should be bounceable.
/// \param testnet whether the result address should be testnet.
/// \return user-friendly address str.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_address_converter_to_user_friendly(
    address: *const TWString,
    bounceable: bool,
    testnet: bool,
) -> *mut TWString {
    let address = try_or_else!(TWString::from_ptr_as_ref(address), std::ptr::null_mut);
    let address_str = try_or_else!(address.as_str(), std::ptr::null_mut);
    let address_ton = try_or_else!(TonAddress::from_str(address_str), std::ptr::null_mut)
        .set_bounceable(bounceable)
        .set_testnet(testnet);

    TWString::from(address_ton.to_string()).into_ptr()
}
