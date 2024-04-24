// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_data_vector::TWDataVector;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_solana::modules::utils::SolanaTransaction;

/// Decode Solana transaction, update the recent blockhash and re-sign the transaction.
///
/// # Warning
///
/// This is a temporary solution. It will be removed when `Solana.proto` supports
/// direct transaction signing.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \param recent_blockhash base58 encoded recent blockhash.
/// \param private_keys list of private keys that should be used to re-sign the transaction.
/// \return serialized `Solana::Proto::SigningOutput`.
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_update_blockhash_and_sign(
    encoded_tx: *const TWString,
    recent_blockhash: *const TWString,
    private_keys: *const TWDataVector,
) -> *mut TWData {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    let recent_blockhash = try_or_else!(
        TWString::from_ptr_as_ref(recent_blockhash),
        std::ptr::null_mut
    );
    let recent_blockhash = try_or_else!(recent_blockhash.as_str(), std::ptr::null_mut);

    let private_keys = try_or_else!(
        TWDataVector::from_ptr_as_ref(private_keys),
        std::ptr::null_mut
    )
    .to_data_vec();

    let output =
        SolanaTransaction::update_blockhash_and_sign(encoded_tx, recent_blockhash, &private_keys);
    let output_proto = try_or_else!(tw_proto::serialize(&output), std::ptr::null_mut);

    TWData::from(output_proto).into_ptr()
}
