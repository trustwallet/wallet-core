// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::{coin_dispatcher, utxo_dispatcher};
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Signs a PSBT (Partially Signed Bitcoin Transaction) specified by the signing input and coin type.
///
/// \param input The serialized data of a signing input (e.g. `TW.BitcoinV2.Proto.PsbtSigningInput`)
/// \param coin The given coin type to sign the PSBT for.
/// \return The serialized data of a `Proto.PsbtSigningOutput` proto object (e.g. `TW.BitcoinV2.Proto.PsbtSigningOutput`).
#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_psbt_sign(input: *const TWData, coin: u32) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let utxo_dispatcher = try_or_else!(utxo_dispatcher(coin), std::ptr::null_mut);
    let (coin_context, _) = try_or_else!(coin_dispatcher(coin), std::ptr::null_mut);

    utxo_dispatcher
        .sign_psbt(&coin_context, input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Plans a PSBT (Partially Signed Bitcoin Transaction).
/// Can be used to get the transaction detailed decoded from PSBT.
///
/// \param input The serialized data of a signing input (e.g. `TW.BitcoinV2.Proto.PsbtSigningInput`)
/// \param coin The given coin type to sign the PSBT for.
/// \return The serialized data of a `Proto.TransactionPlan` proto object (e.g. `TW.BitcoinV2.Proto.TransactionPlan`).
#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_psbt_plan(input: *const TWData, coin: u32) -> *mut TWData {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let utxo_dispatcher = try_or_else!(utxo_dispatcher(coin), std::ptr::null_mut);
    let (coin_context, _) = try_or_else!(coin_dispatcher(coin), std::ptr::null_mut);

    utxo_dispatcher
        .plan_psbt(&coin_context, input_data.as_slice())
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
