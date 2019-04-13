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

static const size_t TWZelcashTAddressSize = 22;

/// Represents a transparent Zelcash address.
TW_EXPORT_STRUCT
struct TWZelcashTAddress {
    /// Address data consisting of two prefix bytes followed by the public key hash.
    uint8_t bytes[22];
};

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressEqual(struct TWZelcashTAddress lhs, struct TWZelcashTAddress rhs);

/// Determines if the data is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressInitWithString(struct TWZelcashTAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressInitWithData(struct TWZelcashTAddress *_Nonnull address, TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
bool TWZelcashTAddressInitWithPublicKey(struct TWZelcashTAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWZelcashTAddressDescription(struct TWZelcashTAddress address);

TW_EXTERN_C_END
