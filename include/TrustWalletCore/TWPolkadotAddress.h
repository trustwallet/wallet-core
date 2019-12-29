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

// TODO: Adjust definitions below

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Polkadot address.
TW_EXPORT_CLASS
struct TWPolkadotAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWPolkadotAddressEqual(struct TWPolkadotAddress *_Nonnull lhs, struct TWPolkadotAddress *_Nonnull rhs);

/// Determines if the string is a valid Polkadot address.
TW_EXPORT_STATIC_METHOD
bool TWPolkadotAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWPolkadotAddress *_Nullable TWPolkadotAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWPolkadotAddress *_Nonnull TWPolkadotAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete address object
TW_EXPORT_METHOD
void TWPolkadotAddressDelete(struct TWPolkadotAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWPolkadotAddressDescription(struct TWPolkadotAddress *_Nonnull address);

/// Returns the public key bytes.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWPolkadotAddressPublicKey(struct TWPolkadotAddress *_Nonnull address);

TW_EXTERN_C_END
