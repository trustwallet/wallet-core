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

/// Represents a Kusama address.
TW_EXPORT_CLASS
struct TWKusamaAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWKusamaAddressEqual(struct TWKusamaAddress *_Nonnull lhs, struct TWKusamaAddress *_Nonnull rhs);

/// Determines if the string is a valid Kusama address.
TW_EXPORT_STATIC_METHOD
bool TWKusamaAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWKusamaAddress *_Nullable TWKusamaAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWKusamaAddress *_Nonnull TWKusamaAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete address object
TW_EXPORT_METHOD
void TWKusamaAddressDelete(struct TWKusamaAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWKusamaAddressDescription(struct TWKusamaAddress *_Nonnull address);

/// Returns the public key bytes.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWKusamaAddressPublicKey(struct TWKusamaAddress *_Nonnull address);

TW_EXTERN_C_END
