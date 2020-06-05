// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWFreeCashAddress.h>

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"


TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a legacy FreeCash address.
TW_EXPORT_STRUCT
struct TWFreeCashCashAddress {
    /// Address data consisting of a prefix byte followed by the public key hash.
    uint8_t bytes[34];
};

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashCashAddressEqual(struct TWFreeCashCashAddress lhs, struct TWFreeCashCashAddress rhs);

/// Determines if the data is a valid FreeCash Cash address.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashCashAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid FreeCash Cash address.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashCashAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashCashAddressInitWithString(struct TWFreeCashCashAddress *_Nonnull address, TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashCashAddressInitWithData(struct TWFreeCashCashAddress *_Nonnull address, TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
void TWFreeCashCashAddressInitWithPublicKey(struct TWFreeCashCashAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWFreeCashCashAddressDescription(struct TWFreeCashCashAddress address);

/// Returns the legacy bitcoin address.
TW_EXPORT_PROPERTY
struct TWFreeCashAddress *_Nonnull TWFreeCashCashAddressLegacyAddress(struct TWFreeCashCashAddress address);

TW_EXTERN_C_END
