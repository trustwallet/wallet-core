// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::transaction_decoder::TransactionDecoder;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Decodes a transaction from a binary representation.
///
/// \param coin coin type.
/// \param tx encoded transaction data.
/// \return serialized protobuf message specific for the given coin.
#[no_mangle]
pub unsafe extern "C" fn tw_transaction_decoder_decode(
    coin: u32,
    tx: *const TWData,
) -> *mut TWData {
    let tx = try_or_else!(TWData::from_ptr_as_ref(tx), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    TransactionDecoder::decode_transaction(coin, tx.as_slice())
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
