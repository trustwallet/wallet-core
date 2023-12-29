// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::any_signer::AnySigner;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Signs a transaction specified by the signing input and coin type.
///
/// \param input The serialized data of a signing input (e.g. TW.Bitcoin.Proto.SigningInput).
/// \param coin The given coin type to sign the transaction for.
/// \return The serialized data of a `SigningOutput` proto object. (e.g. TW.Bitcoin.Proto.SigningOutput).
#[no_mangle]
pub unsafe extern "C" fn tw_any_signer_sign(input: *const TWData, coin: u32) -> *mut TWData {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnySigner::sign(input.as_slice(), coin)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Plans a transaction (for UTXO chains only).
///
/// \param input The serialized data of a signing input
/// \param coin The given coin type to plan the transaction for.
/// \return The serialized data of a `TransactionPlan` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_any_signer_plan(input: *const TWData, coin: u32) -> *mut TWData {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnySigner::plan(input.as_slice(), coin)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Signs a transaction in WalletConnect format.
///
/// \param input The serialized data of a signing input.
/// \param coin The given coin type to plan the transaction for.
/// \return The serialized data of a `TransactionPlan` proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_any_signer_sign_wallet_connect(
    input: *const TWData,
    coin: u32,
) -> *mut TWData {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    AnySigner::sign_wallet_connect(input.as_slice(), coin)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
