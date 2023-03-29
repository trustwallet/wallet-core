// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Solana address helper functions
TW_EXPORT_CLASS
struct TWSolanaAddress;

/// Creates an address from a string representation.
///
/// \param string Non-null pointer to a solana address string
/// \note Should be deleted with \TWSolanaAddressDelete
/// \return Non-null pointer to a Solana address data structure
TW_EXPORT_STATIC_METHOD
struct TWSolanaAddress* _Nullable TWSolanaAddressCreateWithString(TWString* _Nonnull string);

/// Delete the given Solana address
///
/// \param address Non-null pointer to a Solana Address
TW_EXPORT_METHOD
void TWSolanaAddressDelete(struct TWSolanaAddress* _Nonnull address);

/// Derive default token address for token
///
/// \param address Non-null pointer to a Solana Address
/// \param tokenMintAddress Non-null pointer to a token mint address as a string
/// \return Null pointer if the Default token address for a token is not found, valid pointer otherwise
TW_EXPORT_METHOD
TWString* _Nullable TWSolanaAddressDefaultTokenAddress(struct TWSolanaAddress* _Nonnull address, TWString* _Nonnull tokenMintAddress);

/// Returns the address string representation.
///
/// \param address Non-null pointer to a Solana Address
/// \return Non-null pointer to the Solana address string representation
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSolanaAddressDescription(struct TWSolanaAddress *_Nonnull address);

TW_EXTERN_C_END
