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

/// Represents an Ontology address.
TW_EXPORT_CLASS
struct TWOntologyAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWOntologyAddressEqual(struct TWOntologyAddress *_Nonnull lhs, struct TWOntologyAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWOntologyAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a TWDara representaion.
TW_EXPORT_STATIC_METHOD
struct TWOntologyAddress *_Nullable TWOntologyAddressCreateWithData(TWData *_Nonnull data);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWOntologyAddress *_Nullable TWOntologyAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWOntologyAddress *_Nonnull TWOntologyAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWOntologyAddressDelete(struct TWOntologyAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWOntologyAddressDescription(struct TWOntologyAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWOntologyAddressKeyHash(struct TWOntologyAddress *_Nonnull address);

TW_EXTERN_C_END
