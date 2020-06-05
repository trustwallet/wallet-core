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

/// Represents a legacy FreeCash address.
TW_EXPORT_CLASS
struct TWFreeCashAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
    bool TWFreeCashAddressEqual(struct TWFreeCashAddress *_Nonnull lhs, struct TWFreeCashAddress *_Nonnull rhs);

/// Determines if the data is a valid FreeCash address.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid FreeCash address.
TW_EXPORT_STATIC_METHOD
bool TWFreeCashAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithString(TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithData(TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashAddress *_Nullable TWFreeCashAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

TW_EXPORT_METHOD
void TWFreeCashAddressDelete(struct TWFreeCashAddress *_Nonnull address);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWFreeCashAddressDescription(struct TWFreeCashAddress *_Nonnull address);

/// Returns the address prefix.
TW_EXPORT_PROPERTY
uint8_t TWFreeCashAddressPrefix(struct TWFreeCashAddress *_Nonnull address);

/// Returns the keyhash data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWFreeCashAddressKeyhash(struct TWFreeCashAddress *_Nonnull address);

TW_EXTERN_C_END
