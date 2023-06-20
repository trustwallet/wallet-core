// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWHRP.h"
#include "TWString.h"

// TODO: Adjust definitions below

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Substrate address.
TW_EXPORT_CLASS
struct TWSubstrateAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSubstrateAddressEqual(struct TWSubstrateAddress *_Nonnull lhs, struct TWSubstrateAddress *_Nonnull rhs);

/// Determines if the string is a valid Substrate address.
TW_EXPORT_STATIC_METHOD
bool TWSubstrateAddressIsValidString(TWString *_Nonnull string, int32_t network);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSubstrateAddress *_Nullable TWSubstrateAddressCreateWithString(TWString *_Nonnull string, int32_t network);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWSubstrateAddress *_Nonnull TWSubstrateAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, int32_t network);

/// Delete address object
TW_EXPORT_METHOD
void TWSubstrateAddressDelete(struct TWSubstrateAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSubstrateAddressDescription(struct TWSubstrateAddress *_Nonnull address);

TW_EXTERN_C_END