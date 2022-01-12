// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"
#include "TWHRP.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a BIP 0173 address.
TW_EXPORT_CLASS
struct TWSegwitAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSegwitAddressEqual(struct TWSegwitAddress *_Nonnull lhs, struct TWSegwitAddress *_Nonnull rhs);

/// Determines if the string is a valid Bech32 address.
TW_EXPORT_STATIC_METHOD
bool TWSegwitAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSegwitAddress *_Nullable TWSegwitAddressCreateWithString(TWString *_Nonnull string);

/// Creates a segwit-version-0 address from a public key and HRP prefix.
/// Taproot (v>=1) is not supported by this method.
TW_EXPORT_STATIC_METHOD
struct TWSegwitAddress *_Nonnull TWSegwitAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWSegwitAddressDelete(struct TWSegwitAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSegwitAddressDescription(struct TWSegwitAddress *_Nonnull address);

/// Returns the human-readable part.
TW_EXPORT_PROPERTY
enum TWHRP TWSegwitAddressHRP(struct TWSegwitAddress *_Nonnull address);

/// Returns the human-readable part.
TW_EXPORT_PROPERTY
int TWSegwitAddressWitnessVersion(struct TWSegwitAddress *_Nonnull address);

/// Returns the witness program
TW_EXPORT_PROPERTY
TWData *_Nonnull TWSegwitAddressWitnessProgram(struct TWSegwitAddress *_Nonnull address);

TW_EXTERN_C_END
