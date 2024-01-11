// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a Nervos address.
TW_EXPORT_CLASS
struct TWNervosAddress;

/// Compares two addresses for equality.
///
/// \param lhs The first address to compare.
/// \param rhs The second address to compare.
/// \return bool indicating the addresses are equal.
TW_EXPORT_STATIC_METHOD
bool TWNervosAddressEqual(struct TWNervosAddress *_Nonnull lhs, struct TWNervosAddress *_Nonnull rhs);

/// Determines if the string is a valid Nervos address.
///
/// \param string string to validate.
/// \return bool indicating if the address is valid.
TW_EXPORT_STATIC_METHOD
bool TWNervosAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a sring representaion.
///
/// \param string Bech32 string to initialize the address from.
/// \return TWNervosAddress pointer or nullptr if string is invalid.
TW_EXPORT_STATIC_METHOD
struct TWNervosAddress *_Nullable TWNervosAddressCreateWithString(TWString *_Nonnull string);

/// Deletes a Nervos address.
///
/// \param address Address to delete.
TW_EXPORT_METHOD
void TWNervosAddressDelete(struct TWNervosAddress *_Nonnull address);

/// Returns the address string representation.
///
/// \param address Address to get the string representation of.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNervosAddressDescription(struct TWNervosAddress *_Nonnull address);

/// Returns the Code hash
///
/// \param address Address to get the keyhash data of.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNervosAddressCodeHash(struct TWNervosAddress *_Nonnull address);

/// Returns the address hash type
///
/// \param address Address to get the hash type of.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNervosAddressHashType(struct TWNervosAddress *_Nonnull address);

/// Returns the address args data.
///
/// \param address Address to get the args data of.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNervosAddressArgs(struct TWNervosAddress *_Nonnull address);

TW_EXTERN_C_END
