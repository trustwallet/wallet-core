// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::str::FromStr;

use crate::address::Address;
use tw_hash::H256;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, tw_string::TWString, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

/// Returns the checksummed address.
///
/// \param address *non-null* string.
/// \return the checksummed address.
#[tw_ffi(ty = static_function, class = TWEthereum, name = AddressChecksummed)]
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_address_checksummed(
    address: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let address = try_or_else!(TWString::from_ptr_as_ref(address), std::ptr::null_mut);
    let address = try_or_else!(address.as_str(), std::ptr::null_mut);
    let address = try_or_else!(Address::from_str(address), std::ptr::null_mut);
    let checksummed = address.into_checksum_address();
    TWString::from(checksummed).into_ptr()
}

/// Returns the account path from address.
///
/// \param eth_address *non-null* string.
/// \param layer *non-null* string.
/// \param application *non-null* string.
/// \param index *non-null* string.
/// \return the account path.
#[tw_ffi(ty = static_function, class = TWEthereum, name = Eip2645GetPath)]
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_eip2645_get_path(
    eth_address: Nonnull<TWString>,
    layer: Nonnull<TWString>,
    application: Nonnull<TWString>,
    index: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let address = try_or_else!(TWString::from_ptr_as_ref(eth_address), std::ptr::null_mut);
    let address = try_or_else!(address.as_str(), std::ptr::null_mut);
    let address = try_or_else!(Address::from_str(address), std::ptr::null_mut);
    let layer = try_or_else!(TWString::from_ptr_as_ref(layer), std::ptr::null_mut);
    let layer = try_or_else!(layer.as_str(), std::ptr::null_mut);
    let application = try_or_else!(TWString::from_ptr_as_ref(application), std::ptr::null_mut);
    let application = try_or_else!(application.as_str(), std::ptr::null_mut);
    let index = try_or_else!(TWString::from_ptr_as_ref(index), std::ptr::null_mut);
    let index = try_or_else!(index.as_str(), std::ptr::null_mut);
    let path = try_or_else!(
        address.account_path(layer, application, index),
        std::ptr::null_mut
    );
    TWString::from(path).into_ptr()
}

/// Returns EIP-1014 Create2 address
///
/// \param from *non-null* string.
/// \param salt *non-null* data.
/// \param init_code_hash *non-null* data.
/// \return the EIP-1014 Create2 address.
#[tw_ffi(ty = static_function, class = TWEthereum, name = Eip1014Create2Address)]
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_eip1014_create2_address(
    from: Nonnull<TWString>,
    salt: Nonnull<TWData>,
    init_code_hash: Nonnull<TWData>,
) -> NullableMut<TWString> {
    let from = try_or_else!(TWString::from_ptr_as_ref(from), std::ptr::null_mut);
    let from = try_or_else!(from.as_str(), std::ptr::null_mut);
    let salt = try_or_else!(TWData::from_ptr_as_ref(salt), std::ptr::null_mut);
    let salt = try_or_else!(H256::try_from(salt.as_slice()), std::ptr::null_mut);
    let init_code_hash = try_or_else!(TWData::from_ptr_as_ref(init_code_hash), std::ptr::null_mut);
    let init_code_hash = try_or_else!(
        H256::try_from(init_code_hash.as_slice()),
        std::ptr::null_mut
    );
    let address = try_or_else!(
        Address::eip1014_create2_address(from, &salt, &init_code_hash),
        std::ptr::null_mut
    );
    TWString::from(address.into_checksum_address()).into_ptr()
}

/// Returns EIP-1967 proxy init code
///
/// \param logic_address *non-null* string.
/// \param data *non-null* data.
/// \return the EIP-1967 proxy init code.
#[tw_ffi(ty = static_function, class = TWEthereum, name = Eip1967ProxyInitCode)]
#[no_mangle]
pub unsafe extern "C" fn tw_ethereum_eip1967_proxy_init_code(
    logic_address: Nonnull<TWString>,
    data: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let logic_address = try_or_else!(TWString::from_ptr_as_ref(logic_address), std::ptr::null_mut);
    let logic_address = try_or_else!(logic_address.as_str(), std::ptr::null_mut);
    let data = try_or_else!(TWData::from_ptr_as_ref(data), std::ptr::null_mut);
    let init_code = try_or_else!(
        Address::eip_1967_proxy_init_code(logic_address, data.as_slice()),
        std::ptr::null_mut
    );
    TWData::from(init_code).into_ptr()
}
