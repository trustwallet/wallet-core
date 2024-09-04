// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use std::str::FromStr;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_solana::address::SolanaAddress;
use tw_solana::defined_addresses::{TOKEN_2022_PROGRAM_ID_ADDRESS, TOKEN_PROGRAM_ID_ADDRESS};
use tw_solana::program::stake_program::StakeProgram;

/// Derive default token address for token
///
/// \param address Non-null pointer to a Solana Address
/// \param token_mint_address Non-null pointer to a token mint address as a string
/// \return Null pointer if the Default token address for a token is not found, valid pointer otherwise
#[no_mangle]
pub unsafe extern "C" fn tw_solana_address_default_token_address(
    address: *const TWString,
    token_mint_address: *const TWString,
) -> *mut TWString {
    tw_solana_address_token_address_impl(address, token_mint_address, *TOKEN_PROGRAM_ID_ADDRESS)
}

/// Derive token 2022 address for token
///
/// \param address Non-null pointer to a Solana Address
/// \param token_mint_address Non-null pointer to a token mint address as a string
/// \return Null pointer if the token 2022 address for a token is not found, valid pointer otherwise
#[no_mangle]
pub unsafe extern "C" fn tw_solana_address_token_2022_address(
    address: *const TWString,
    token_mint_address: *const TWString,
) -> *mut TWString {
    tw_solana_address_token_address_impl(
        address,
        token_mint_address,
        *TOKEN_2022_PROGRAM_ID_ADDRESS,
    )
}

unsafe fn tw_solana_address_token_address_impl(
    address: *const TWString,
    token_mint_address: *const TWString,
    token_address: SolanaAddress,
) -> *mut TWString {
    let main_address = try_or_else!(TWString::from_ptr_as_ref(address), std::ptr::null_mut);
    let main_address = try_or_else!(main_address.as_str(), std::ptr::null_mut);
    let main_address = try_or_else!(SolanaAddress::from_str(main_address), std::ptr::null_mut);

    let token_mint_address = try_or_else!(
        TWString::from_ptr_as_ref(token_mint_address),
        std::ptr::null_mut
    );
    let token_mint_address = try_or_else!(token_mint_address.as_str(), std::ptr::null_mut);
    let token_mint_address = try_or_else!(
        SolanaAddress::from_str(token_mint_address),
        std::ptr::null_mut
    );

    let associated_token_address = try_or_else!(
        StakeProgram::get_associated_token_address(main_address, token_address, token_mint_address),
        std::ptr::null_mut
    );

    TWString::from(associated_token_address.to_string()).into_ptr()
}
