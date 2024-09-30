// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::any_address::AnyAddress;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::prefix::{AddressPrefix, BitcoinBase58Prefix};
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::tw_derivation::TWDerivation;
use tw_keypair::ffi::pubkey::TWPublicKey;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

/// Represents an address in Rust for almost any blockchain.
pub struct TWAnyAddress(AnyAddress);

impl RawPtrTrait for TWAnyAddress {}

/// Determines if the string is a valid Any address.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \return bool indicating if the address is valid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_is_valid(string: *const TWString, coin: u32) -> bool {
    let string = try_or_false!(TWString::from_ptr_as_ref(string));
    let string = try_or_false!(string.as_str());
    let coin = try_or_false!(CoinType::try_from(coin));

    AnyAddress::is_valid(coin, string, None)
}

/// Determines if the string is a valid Any address with the given hrp.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \param hrp explicit given hrp of the given address.
/// \return bool indicating if the address is valid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_is_valid_bech32(
    string: *const TWString,
    coin: u32,
    hrp: *const TWString,
) -> bool {
    let string = try_or_false!(TWString::from_ptr_as_ref(string));
    let string = try_or_false!(string.as_str());

    let hrp = try_or_false!(TWString::from_ptr_as_ref(hrp));
    let hrp = try_or_false!(hrp.as_str());

    let coin = try_or_false!(CoinType::try_from(coin));

    let prefix = AddressPrefix::Hrp(hrp.to_string());
    AnyAddress::is_valid(coin, string, Some(prefix))
}

/// Determines if the string is a valid Any address with the given Base58 prefixes.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \param p2pkh pay-to-public-key-hash address prefix.
/// \param p2sh pay-to-script-hash address prefix.
/// \return bool indicating if the address is valid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_is_valid_base58(
    string: *const TWString,
    coin: u32,
    p2pkh: u8,
    p2sh: u8,
) -> bool {
    let string = try_or_false!(TWString::from_ptr_as_ref(string));
    let string = try_or_false!(string.as_str());

    let coin = try_or_false!(CoinType::try_from(coin));

    let prefix = AddressPrefix::BitcoinBase58(BitcoinBase58Prefix { p2pkh, p2sh });
    AnyAddress::is_valid(coin, string, Some(prefix))
}

/// Creates an address from a string representation and a coin type. Must be deleted with `TWAnyAddressDelete` after use.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \return `TWAnyAddress` pointer or nullptr if address and coin are invalid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_create_with_string(
    string: *const TWString,
    coin: u32,
) -> *mut TWAnyAddress {
    let string = try_or_else!(TWString::from_ptr_as_ref(string), std::ptr::null_mut);
    let string = try_or_else!(string.as_str(), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnyAddress::with_string(coin, string, None)
        .map(|any_address| TWAnyAddress(any_address).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Creates an address from a public key and derivation option.
///
/// \param public_key derivates the address from the public key.
/// \param coin coin type of the address.
/// \param derivation the custom derivation to use.
/// \return `TWAnyAddress` pointer or nullptr if public key is invalid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_create_with_public_key_derivation(
    public_key: *mut TWPublicKey,
    coin: u32,
    derivation: u32,
) -> *mut TWAnyAddress {
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_ref(public_key), std::ptr::null_mut);
    let derivation = try_or_else!(TWDerivation::from_repr(derivation), std::ptr::null_mut);
    let derivation = Derivation::from(derivation);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnyAddress::with_public_key(coin, public_key.as_ref().clone(), derivation, None)
        .map(|any_address| TWAnyAddress(any_address).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Creates an bech32 address from a public key and a given hrp.
///
/// \param public_key derivates the address from the public key.
/// \param coin coin type of the address.
/// \param hrp hrp of the address.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_create_bech32_with_public_key(
    public_key: *mut TWPublicKey,
    coin: u32,
    hrp: *const TWString,
) -> *mut TWAnyAddress {
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_ref(public_key), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    let hrp = try_or_else!(TWString::from_ptr_as_ref(hrp), std::ptr::null_mut);
    let hrp = try_or_else!(hrp.as_str(), std::ptr::null_mut);

    let prefix = AddressPrefix::Hrp(hrp.to_string());
    AnyAddress::with_public_key(
        coin,
        public_key.as_ref().clone(),
        Derivation::default(),
        Some(prefix),
    )
    .map(|any_address| TWAnyAddress(any_address).into_ptr())
    .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Creates an Base58 Bitcoin address from a public key and a given hrp.
///
/// \param public_key derivates the address from the public key.
/// \param coin coin type of the address.
/// \param p2pkh pay-to-public-key-hash address prefix.
/// \param p2sh pay-to-script-hash address prefix.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_create_base58_with_public_key(
    public_key: *mut TWPublicKey,
    coin: u32,
    p2pkh: u8,
    p2sh: u8,
) -> *mut TWAnyAddress {
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_ref(public_key), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    let prefix = AddressPrefix::BitcoinBase58(BitcoinBase58Prefix { p2pkh, p2sh });
    AnyAddress::with_public_key(
        coin,
        public_key.as_ref().clone(),
        Derivation::default(),
        Some(prefix),
    )
    .map(|any_address| TWAnyAddress(any_address).into_ptr())
    .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Deletes an address.
///
/// \param address address to delete.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_delete(address: *mut TWAnyAddress) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWAnyAddress::from_ptr(address);
}

/// Returns the address string representation.
///
/// \param address address to get the string representation of.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_description(address: *const TWAnyAddress) -> *mut TWString {
    // Take the ownership back to rust and drop the owner.
    let address = try_or_else!(TWAnyAddress::from_ptr_as_ref(address), std::ptr::null_mut);

    let description = address.0.description().to_string();
    TWString::from(description).into_ptr()
}

/// Returns underlying data (public key or key hash)
///
/// \param address address to get the data of.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_data(address: *const TWAnyAddress) -> *mut TWData {
    // Take the ownership back to rust and drop the owner.
    let address = try_or_else!(TWAnyAddress::from_ptr_as_ref(address), std::ptr::null_mut);

    let data = try_or_else!(address.0.get_data(), std::ptr::null_mut);
    TWData::from(data).into_ptr()
}

/// Creates an address from a string representation and a coin type. Must be deleted with `TWAnyAddressDelete` after use.
/// This function does not check if the address belongs to the given chain.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \return `TWAnyAddress` pointer or nullptr if address and coin are invalid.
///
/// # Warning
///
/// This function should only be used when address prefix is unavailable to be passed to this function.
/// Consider using `tw_any_address_create_<PREFIX>_with_string` if the prefix is known.
/// Please note that this function should be removed when all chains are migrated to Rust.
#[no_mangle]
pub unsafe extern "C" fn tw_any_address_create_with_string_unchecked(
    string: *const TWString,
    coin: u32,
) -> *mut TWAnyAddress {
    let string = try_or_else!(TWString::from_ptr_as_ref(string), std::ptr::null_mut);
    let string = try_or_else!(string.as_str(), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnyAddress::with_string_unchecked(coin, string)
        .map(|any_address| TWAnyAddress(any_address).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
