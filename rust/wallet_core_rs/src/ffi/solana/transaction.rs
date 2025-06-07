// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_data_vector::TWDataVector;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::{Nonnull, NullableMut, RawPtrTrait};
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
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = UpdateBlockhashAndSign)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_update_blockhash_and_sign(
    encoded_tx: Nonnull<TWString>,
    recent_blockhash: Nonnull<TWString>,
    private_keys: Nonnull<TWDataVector>,
) -> NullableMut<TWData> {
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

/// Try to find a `ComputeBudgetInstruction::SetComputeUnitPrice` instruction in the given transaction,
/// and returns the specified Unit Price.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \return nullable Unit Price as a decimal string. Null if no instruction found.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = GetComputeUnitPrice)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_get_compute_unit_price(
    encoded_tx: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    match SolanaTransaction::get_compute_unit_price(encoded_tx) {
        Ok(Some(price)) => TWString::from(price.to_string()).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}

/// Try to find a `ComputeBudgetInstruction::SetComputeUnitLimit` instruction in the given transaction,
/// and returns the specified Unit Limit.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \return nullable Unit Limit as a decimal string. Null if no instruction found.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = GetComputeUnitLimit)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_get_compute_unit_limit(
    encoded_tx: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    match SolanaTransaction::get_compute_unit_limit(encoded_tx) {
        Ok(Some(limit)) => TWString::from(limit.to_string()).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}

/// Adds or updates a `ComputeBudgetInstruction::SetComputeUnitPrice` instruction of the given transaction,
/// and returns the updated transaction.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \price Unit Price as a decimal string.
/// \return base64 encoded Solana transaction. Null if an error occurred.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = SetComputeUnitPrice)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_set_compute_unit_price(
    encoded_tx: Nonnull<TWString>,
    price: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    let price = try_or_else!(TWString::from_ptr_as_ref(price), std::ptr::null_mut);
    let price = try_or_else!(price.as_str(), std::ptr::null_mut);
    let price = try_or_else!(price.parse(), std::ptr::null_mut);

    match SolanaTransaction::set_compute_unit_price(encoded_tx, price) {
        Ok(updated_tx) => TWString::from(updated_tx).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}

/// Adds or updates a `ComputeBudgetInstruction::SetComputeUnitLimit` instruction of the given transaction,
/// and returns the updated transaction.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \limit Unit Limit as a decimal string.
/// \return base64 encoded Solana transaction. Null if an error occurred.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = SetComputeUnitLimit)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_set_compute_unit_limit(
    encoded_tx: Nonnull<TWString>,
    limit: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    let limit = try_or_else!(TWString::from_ptr_as_ref(limit), std::ptr::null_mut);
    let limit = try_or_else!(limit.as_str(), std::ptr::null_mut);
    let limit = try_or_else!(limit.parse(), std::ptr::null_mut);

    match SolanaTransaction::set_compute_unit_limit(encoded_tx, limit) {
        Ok(updated_tx) => TWString::from(updated_tx).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}

/// Adds fee payer to the given transaction, and returns the updated transaction.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \param fee_payer fee payer account address. Must be a base58 encoded public key. It must NOT be in the account list yet.
/// \return base64 encoded Solana transaction. Null if an error occurred.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = SetFeePayer)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_set_fee_payer(
    encoded_tx: Nonnull<TWString>,
    fee_payer: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    let fee_payer = try_or_else!(TWString::from_ptr_as_ref(fee_payer), std::ptr::null_mut);
    let fee_payer = try_or_else!(fee_payer.as_str(), std::ptr::null_mut);
    let fee_payer = try_or_else!(fee_payer.parse(), std::ptr::null_mut);

    match SolanaTransaction::set_fee_payer(encoded_tx, fee_payer) {
        Ok(updated_tx) => TWString::from(updated_tx).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}

/// Inserts an instruction to the given transaction at the specified position, returning the updated transaction.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \param insert_at index where the instruction should be inserted. If you don't care about the position, use -1.
/// \param instruction json encoded instruction. Here is an example: {"programId":"11111111111111111111111111111111","accounts":[{"pubkey":"YUz1AupPEy1vttBeDS7sXYZFhQJppcXMzjDiDx18Srf","isSigner":true,"isWritable":true},{"pubkey":"d8DiHEeHKdXkM2ZupT86mrvavhmJwUZjHPCzMiB5Lqb","isSigner":false,"isWritable":true}],"data":"3Bxs4Z6oyhaczjLK"}
/// \return base64 encoded Solana transaction. Null if an error occurred.
#[tw_ffi(ty = static_function, class = TWSolanaTransaction, name = InsertInstruction)]
#[no_mangle]
pub unsafe extern "C" fn tw_solana_transaction_insert_instruction(
    encoded_tx: Nonnull<TWString>,
    insert_at: i32,
    instruction: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let encoded_tx = try_or_else!(TWString::from_ptr_as_ref(encoded_tx), std::ptr::null_mut);
    let encoded_tx = try_or_else!(encoded_tx.as_str(), std::ptr::null_mut);

    let instruction = try_or_else!(TWString::from_ptr_as_ref(instruction), std::ptr::null_mut);
    let instruction = try_or_else!(instruction.as_str(), std::ptr::null_mut);

    match SolanaTransaction::insert_instruction(encoded_tx, insert_at, instruction) {
        Ok(updated_tx) => TWString::from(updated_tx).into_ptr(),
        _ => std::ptr::null_mut(),
    }
}
