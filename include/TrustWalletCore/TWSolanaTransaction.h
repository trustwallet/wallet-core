// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWDataVector.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWSolanaTransaction;

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
TW_EXPORT_STATIC_METHOD TWData *_Nullable TWSolanaTransactionUpdateBlockhashAndSign(TWString *_Nonnull encodedTx, TWString *_Nonnull recentBlockhash, struct TWDataVector *_Nonnull privateKeys);

/// Try to find a `ComputeBudgetInstruction::SetComputeUnitPrice` instruction in the given transaction,
/// and returns the specified Unit Price.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \return nullable Unit Price as a decimal string. Null if no instruction found.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionGetComputeUnitPrice(TWString *_Nonnull encodedTx);

/// Try to find a `ComputeBudgetInstruction::SetComputeUnitLimit` instruction in the given transaction,
/// and returns the specified Unit Limit.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \return nullable Unit Limit as a decimal string. Null if no instruction found.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionGetComputeUnitLimit(TWString *_Nonnull encodedTx);

/// Adds or updates a `ComputeBudgetInstruction::SetComputeUnitPrice` instruction of the given transaction,
/// and returns the updated transaction.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \price Unit Price as a decimal string.
/// \return base64 encoded Solana transaction. Null if an error occurred.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionSetComputeUnitPrice(TWString *_Nonnull encodedTx, TWString *_Nonnull price);

/// Adds or updates a `ComputeBudgetInstruction::SetComputeUnitLimit` instruction of the given transaction,
/// and returns the updated transaction.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \limit Unit Limit as a decimal string.
/// \return base64 encoded Solana transaction. Null if an error occurred.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionSetComputeUnitLimit(TWString *_Nonnull encodedTx, TWString *_Nonnull limit);

/// Adds fee payer to the given transaction, and returns the updated transaction.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \param fee_payer fee payer account address. Must be a base58 encoded public key. It must NOT be in the account list yet.
/// \return base64 encoded Solana transaction. Null if an error occurred.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionSetFeePayer(TWString *_Nonnull encodedTx, TWString *_Nonnull feePayer);

/// Adds an instruction to the given transaction, and returns the updated transaction.
/// 
/// \param encoded_tx base64 encoded Solana transaction.
/// \param instruction json encoded instruction. Here is an example: {"programId":"11111111111111111111111111111111","accounts":[{"pubkey":"YUz1AupPEy1vttBeDS7sXYZFhQJppcXMzjDiDx18Srf","isSigner":true,"isWritable":true},{"pubkey":"d8DiHEeHKdXkM2ZupT86mrvavhmJwUZjHPCzMiB5Lqb","isSigner":false,"isWritable":true}],"data":"3Bxs4Z6oyhaczjLK"}
/// \return base64 encoded Solana transaction. Null if an error occurred.
TW_EXPORT_STATIC_METHOD TWString *_Nullable TWSolanaTransactionAddInstruction(TWString *_Nonnull encodedTx, TWString *_Nonnull instruction);

TW_EXTERN_C_END
