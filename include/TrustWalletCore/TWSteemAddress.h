// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

TW_EXPORT_ENUM(uint32_t)
enum TWSteemAddressType {
	TWSteemAddressTypeMainNet = 0,
	TWSteemAddressTypeTestNet = 1,
};

/// Represents a Bravo address.
TW_EXPORT_CLASS
struct TWSteemAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSteemAddressEqual(struct TWSteemAddress *_Nonnull lhs, struct TWSteemAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWSteemAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nullable TWSteemAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nonnull TWSteemAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, TWSteemAddressType type);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nullable TWSteemAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWSteemAddressType type);

TW_EXPORT_METHOD
void TWSteemAddressDelete(struct TWSteemAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSteemAddressDescription(struct TWSteemAddress *_Nonnull address);

TW_EXTERN_C_END