// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents Any blockchain address.
TW_EXPORT_CLASS
struct TWAnyAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs);

/// Determines if the string is a valid Any address.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin);

/// Creates an address from a string representation.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string, enum TWCoinType coin);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin);

TW_EXPORT_METHOD
void TWAnyAddressDelete(struct TWAnyAddress* _Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAnyAddressDescription(struct TWAnyAddress* _Nonnull address);

/// Returns coin type of address.
TW_EXPORT_PROPERTY
enum TWCoinType TWAnyAddressCoin(struct TWAnyAddress* _Nonnull address);

/// Returns underlaying data (public key or key hash)
TW_EXPORT_PROPERTY
TWData* _Nonnull TWAnyAddressData(struct TWAnyAddress* _Nonnull address);

TW_EXTERN_C_END
