// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an address in C++ for almost any blockchain.
TW_EXPORT_CLASS
struct TWAnyAddress;

/// Compares two addresses for equality.
///
/// \param lhs The first address to compare.
/// \param rhs The second address to compare.
/// \return bool indicating the addresses are equal.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressEqual(struct TWAnyAddress* _Nonnull lhs, struct TWAnyAddress* _Nonnull rhs);

/// Determines if the string is a valid Any address.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \return bool indicating if the address is valid.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressIsValid(TWString* _Nonnull string, enum TWCoinType coin);

/// Creates an address from a string representation and a coin type. Must be deleted with TWAnyAddressDelete after use.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \return TWAnyAddress pointer or nullptr if address and coin are invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string, enum TWCoinType coin);

/// Creates an address from a public key.
///
/// \param publicKey derivates the address from the public key.
/// \param coin coin type of the address.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin);

/// Deletes an address.
///
/// \param address address to delete.
TW_EXPORT_METHOD
void TWAnyAddressDelete(struct TWAnyAddress* _Nonnull address);

/// Returns the address string representation.
///
/// \param address address to get the string representation of.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAnyAddressDescription(struct TWAnyAddress* _Nonnull address);

/// Returns coin type of address.
///
/// \param address address to get the coin type of.
TW_EXPORT_PROPERTY
enum TWCoinType TWAnyAddressCoin(struct TWAnyAddress* _Nonnull address);

/// Returns underlaying data (public key or key hash)
///
/// \param address address to get the data of.
TW_EXPORT_PROPERTY
TWData* _Nonnull TWAnyAddressData(struct TWAnyAddress* _Nonnull address);

TW_EXTERN_C_END
