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

struct TWPublicKeySecp256k1;

/// Represents a Tendermint address.
TW_EXPORT_CLASS
struct TWTendermintAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWTendermintAddressEqual(struct TWTendermintAddress *_Nonnull lhs, struct TWTendermintAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWTendermintAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWTendermintAddress *_Nullable TWTendermintAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWTendermintAddress *_Nullable TWTendermintAddressCreateWithKeyHash(enum TWHRP hrp, TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWTendermintAddress *_Nullable TWTendermintAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKeySecp256k1 publicKey);

TW_EXPORT_METHOD
void TWTendermintAddressDelete(struct TWTendermintAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWTendermintAddressDescription(struct TWTendermintAddress *_Nonnull address);

/// Wheter this is a test net address.
TW_EXPORT_PROPERTY
enum TWHRP TWTendermintAddressHRP(struct TWTendermintAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWTendermintAddressKeyHash(struct TWTendermintAddress *_Nonnull address);

TW_EXTERN_C_END
