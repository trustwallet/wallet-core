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

static const size_t TWTezosAddressSize = 22;

/// Represents a transparent Zcash address.
TW_EXPORT_STRUCT
struct TWTezosAddress {
    /// Address data consisting of two prefix bytes followed by the public key hash.
    uint8_t bytes[22];
};

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address);

/// Determines if the data is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey);

/// Returns the address data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress address);

TW_EXTERN_C_END
