// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWSolanaAddress;

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSolanaAddress* _Nullable TWSolanaAddressCreateWithString(TWString* _Nonnull string);

TW_EXPORT_METHOD
void TWSolanaAddressDelete(struct TWSolanaAddress* _Nonnull address);

/// Derive default token address for token
TW_EXPORT_METHOD
TWString* _Nullable TWSolanaAddressDefaultTokenAddress(struct TWSolanaAddress* _Nonnull address, TWString* _Nonnull tokenMintAddress);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSolanaAddressDescription(struct TWSolanaAddress *_Nonnull address);

TW_EXTERN_C_END
