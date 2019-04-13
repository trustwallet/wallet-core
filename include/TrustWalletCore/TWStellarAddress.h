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

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Stellar address.
TW_EXPORT_CLASS
struct TWStellarAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWStellarAddressEqual(struct TWStellarAddress *_Nonnull lhs, struct TWStellarAddress *_Nonnull rhs);

/// Determines if the string is a valid Stellar address.
TW_EXPORT_STATIC_METHOD
bool TWStellarAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWStellarAddress *_Nullable TWStellarAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWStellarAddress *_Nonnull TWStellarAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWStellarAddressDelete(struct TWStellarAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWStellarAddressDescription(struct TWStellarAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWStellarAddressKeyHash(struct TWStellarAddress *_Nonnull address);

TW_EXTERN_C_END
