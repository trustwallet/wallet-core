// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

#include <TrustWalletCore/TWBitcoinAddress.h>

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a legacy Bitcoin address.
TW_EXPORT_STRUCT
struct TWBitcoinCashAddress {
    /// Address data consisting of a prefix byte followed by the public key hash.
    uint8_t bytes[34];
};

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinCashAddressEqual(struct TWBitcoinCashAddress lhs, struct TWBitcoinCashAddress rhs);

/// Determines if the data is a valid Bitcoin Cash address.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinCashAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin Cash address.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinCashAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinCashAddressInitWithString(struct TWBitcoinCashAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinCashAddressInitWithData(struct TWBitcoinCashAddress *_Nonnull address, TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
void TWBitcoinCashAddressInitWithPublicKey(struct TWBitcoinCashAddress *_Nonnull address, struct TWPublicKey publicKey);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBitcoinCashAddressDescription(struct TWBitcoinCashAddress address);

/// Returns the address data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWBitcoinCashAddressData(struct TWBitcoinCashAddress address);

/// Returns the legacy bitcoin address.
TW_EXPORT_PROPERTY
struct TWBitcoinAddress TWBitcoinCashAddressLegacyAddress(struct TWBitcoinCashAddress address);

TW_EXTERN_C_END
