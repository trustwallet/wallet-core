// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWHRP.h"
#include "TWString.h"

// TODO: Adjust definitions below

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an SampleCoin address.
TW_EXPORT_CLASS
struct TWSampleCoinAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSampleCoinAddressEqual(struct TWSampleCoinAddress *_Nonnull lhs, struct TWSampleCoinAddress *_Nonnull rhs);

/// Determines if the string is a valid SampleCoin address.
TW_EXPORT_STATIC_METHOD
bool TWSampleCoinAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSampleCoinAddress *_Nullable TWSampleCoinAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWSampleCoinAddress *_Nonnull TWSampleCoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete address object
TW_EXPORT_METHOD
void TWSampleCoinAddressDelete(struct TWSampleCoinAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSampleCoinAddressDescription(struct TWSampleCoinAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWSampleCoinAddressKeyHash(struct TWSampleCoinAddress *_Nonnull address);

TW_EXTERN_C_END
