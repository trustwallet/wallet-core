// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a legacy Groestlcoin address.
TW_EXPORT_CLASS
struct TWGroestlcoinAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWGroestlcoinAddressEqual(struct TWGroestlcoinAddress *_Nonnull lhs, struct TWGroestlcoinAddress *_Nonnull rhs);

/// Determines if the string is a valid Groestlcoin address.
TW_EXPORT_STATIC_METHOD
bool TWGroestlcoinAddressIsValidString(TWString *_Nonnull string);

/// Create an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinAddress *_Nullable TWGroestlcoinAddressCreateWithString(TWString *_Nonnull string);

/// Create an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinAddress *_Nonnull TWGroestlcoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

TW_EXPORT_METHOD
void TWGroestlcoinAddressDelete(struct TWGroestlcoinAddress *_Nonnull address);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWGroestlcoinAddressDescription(struct TWGroestlcoinAddress *_Nonnull address);

TW_EXTERN_C_END
