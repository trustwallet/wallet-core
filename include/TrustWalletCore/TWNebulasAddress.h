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

/// Represents an Nebulas address.
TW_EXPORT_CLASS
struct TWNebulasAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNebulasAddressEqual(struct TWNebulasAddress *_Nonnull lhs, struct TWNebulasAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWNebulasAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNebulasAddress *_Nullable TWNebulasAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWNebulasAddress *_Nullable TWNebulasAddressCreateWithKeyHash(TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNebulasAddress *_Nonnull TWNebulasAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

TW_EXPORT_METHOD
void TWNebulasAddressDelete(struct TWNebulasAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNebulasAddressDescription(struct TWNebulasAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNebulasAddressKeyHash(struct TWNebulasAddress *_Nonnull address);

TW_EXTERN_C_END
