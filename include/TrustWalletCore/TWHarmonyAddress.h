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

/// Represents an Harmony address.
TW_EXPORT_CLASS
struct TWHarmonyAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWHarmonyAddressEqual(struct TWHarmonyAddress *_Nonnull lhs,
                           struct TWHarmonyAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWHarmonyAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWHarmonyAddress *_Nullable TWHarmonyAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWHarmonyAddress *_Nullable TWHarmonyAddressCreateWithKeyHash(TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWHarmonyAddress *_Nonnull TWHarmonyAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWHarmonyAddressDelete(struct TWHarmonyAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWHarmonyAddressDescription(struct TWHarmonyAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWHarmonyAddressKeyHash(struct TWHarmonyAddress *_Nonnull address);

TW_EXTERN_C_END
