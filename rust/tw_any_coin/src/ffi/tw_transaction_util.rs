// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::transaction_util::TransactionUtil;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Calculate the TX hash of a transaction.
///
/// \param coin coin type.
/// \param encoded_tx encoded transaction data.
/// \return The TX hash of a transaction, If the input is invalid or the chain is unsupported, null is returned.
#[no_mangle]
pub unsafe extern "C" fn tw_transaction_util_calc_tx_hash(
    coin: u32,
    encoded_tx: *const TWString,
) -> *mut TWString {
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    TransactionUtil::calc_tx_hash(coin, encoded_tx)
        .map(|output| TWString::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
