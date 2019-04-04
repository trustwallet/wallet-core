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

/// Represents a legacy Bitcoin address.
TW_EXPORT_STRUCT
struct TWBitcoinAddress {
    /// Address data consisting of a prefix byte followed by the public key hash.
    uint8_t bytes[21];
};

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressEqual(struct TWBitcoinAddress lhs, struct TWBitcoinAddress rhs);

/// Determines if the data is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressInitWithString(struct TWBitcoinAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressInitWithData(struct TWBitcoinAddress *_Nonnull address, TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
void TWBitcoinAddressInitWithPublicKey(struct TWBitcoinAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBitcoinAddressDescription(struct TWBitcoinAddress address);

/// Returns the keyhash data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWBitcoinAddressKeyhash(struct TWBitcoinAddress address);

TW_EXTERN_C_END
