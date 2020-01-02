// Copyright © 2017-2020 Trust Wallet.
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

/// Represents a Tezos address.
TW_EXPORT_CLASS
struct TWTezosAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressEqual(struct TWTezosAddress *_Nonnull lhs, struct TWTezosAddress *_Nonnull rhs);

/// Determines if the string is a valid Tezos address.
TW_EXPORT_STATIC_METHOD
bool TWTezosAddressIsValidString(TWString *_Nonnull string);

/// Derives an originated account from the given inputs.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWTezosAddressDeriveOriginatedAddress(TWString *_Nonnull operationHash, int operationIndex);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWTezosAddress *_Nullable TWTezosAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWTezosAddress *_Nonnull TWTezosAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWTezosAddressDelete(struct TWTezosAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWTezosAddressKeyHash(struct TWTezosAddress *_Nonnull address);

TW_EXTERN_C_END
