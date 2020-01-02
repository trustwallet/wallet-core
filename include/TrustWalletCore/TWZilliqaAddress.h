// Copyright © 2017-2020 Trust Wallet.
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

/// Represents a Zilliqa address.
TW_EXPORT_CLASS
struct TWZilliqaAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWZilliqaAddressEqual(struct TWZilliqaAddress *_Nonnull lhs, struct TWZilliqaAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWZilliqaAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWZilliqaAddress *_Nullable TWZilliqaAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from key hash.
TW_EXPORT_STATIC_METHOD
struct TWZilliqaAddress *_Nullable TWZilliqaAddressCreateWithKeyHash(TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWZilliqaAddress *_Nonnull TWZilliqaAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWZilliqaAddressDelete(struct TWZilliqaAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWZilliqaAddressDescription(struct TWZilliqaAddress *_Nonnull address);

/// Returns the key hash string with checksum.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWZilliqaAddressKeyHash(struct TWZilliqaAddress *_Nonnull address);

TW_EXTERN_C_END
