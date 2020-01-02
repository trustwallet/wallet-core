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

/// Represents an Waves address.
TW_EXPORT_CLASS
struct TWWavesAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWWavesAddressEqual(struct TWWavesAddress *_Nonnull lhs, struct TWWavesAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWWavesAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWWavesAddress *_Nullable TWWavesAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWWavesAddress *_Nonnull TWWavesAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWWavesAddressDelete(struct TWWavesAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWWavesAddressDescription(struct TWWavesAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWWavesAddressKeyHash(struct TWWavesAddress *_Nonnull address);

TW_EXTERN_C_END
