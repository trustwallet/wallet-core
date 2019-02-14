// Copyright © 2017-2019 Trust.
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

/// Represents a BIP 0173 address.
TW_EXPORT_CLASS
struct TWBech32Address;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWBech32AddressEqual(struct TWBech32Address *_Nonnull lhs, struct TWBech32Address *_Nonnull rhs);

/// Determines if the string is a valid Bech32 address.
TW_EXPORT_STATIC_METHOD
bool TWBech32AddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWBech32Address *_Nullable TWBech32AddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a raw data representaion.
TW_EXPORT_STATIC_METHOD
struct TWBech32Address *_Nullable TWBech32AddressCreateWithData(enum TWHRP hrp, TWData *_Nonnull data);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWBech32Address *_Nullable TWBech32AddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey publicKey);

TW_EXPORT_METHOD
void TWBech32AddressDelete(struct TWBech32Address *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBech32AddressDescription(struct TWBech32Address *_Nonnull address);

/// Returns the human-readable part.
TW_EXPORT_PROPERTY
enum TWHRP TWBech32AddressHRP(struct TWBech32Address *_Nonnull address);

/// Returns the witness program
TW_EXPORT_PROPERTY
TWData *_Nonnull TWBech32AddressWitnessProgram(struct TWBech32Address *_Nonnull address);

TW_EXTERN_C_END
