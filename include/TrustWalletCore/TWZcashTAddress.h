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

static const size_t TWZcashTAddressSize = 22;

/// Represents a transparent Zcash address.
TW_EXPORT_STRUCT
struct TWZcashTAddress {
    /// Address data consisting of two prefix bytes followed by the public key hash.
    uint8_t bytes[22];
};

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressEqual(struct TWZcashTAddress lhs, struct TWZcashTAddress rhs);

/// Determines if the data is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressInitWithString(struct TWZcashTAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressInitWithData(struct TWZcashTAddress *_Nonnull address, TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressInitWithPublicKey(struct TWZcashTAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWZcashTAddressDescription(struct TWZcashTAddress address);

TW_EXTERN_C_END
