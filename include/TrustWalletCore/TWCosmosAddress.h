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

/// Represents a Cosmos address.
TW_EXPORT_CLASS
struct TWCosmosAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWCosmosAddressEqual(struct TWCosmosAddress *_Nonnull lhs, struct TWCosmosAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWCosmosAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWCosmosAddress *_Nullable TWCosmosAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWCosmosAddress *_Nullable TWCosmosAddressCreateWithKeyHash(enum TWHRP hrp, TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWCosmosAddress *_Nullable TWCosmosAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWCosmosAddressDelete(struct TWCosmosAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWCosmosAddressDescription(struct TWCosmosAddress *_Nonnull address);

/// Wheter this is a test net address.
TW_EXPORT_PROPERTY
enum TWHRP TWCosmosAddressHRP(struct TWCosmosAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWCosmosAddressKeyHash(struct TWCosmosAddress *_Nonnull address);

TW_EXTERN_C_END
