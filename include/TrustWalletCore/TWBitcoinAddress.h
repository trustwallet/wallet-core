// Copyright © 2017-2023 Trust Wallet.
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

/// Represents a legacy Bitcoin address in C++.
TW_EXPORT_CLASS
struct TWBitcoinAddress;

/// Compares two addresses for equality.
///
/// \param lhs The first address to compare.
/// \param rhs The second address to compare.
/// \return bool indicating the addresses are equal.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressEqual(struct TWBitcoinAddress *_Nonnull lhs, struct TWBitcoinAddress *_Nonnull rhs);

/// Determines if the data is a valid Bitcoin address.
///
/// \param data data to validate.
/// \return bool indicating if the address data is valid.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
///
/// \param string string to validate.
/// \return bool indicating if the address string is valid.
TW_EXPORT_STATIC_METHOD
bool TWBitcoinAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a Base58 sring. Must be deleted with TWBitcoinAddressDelete after use.
///
/// \param string Base58 string to initialize the address from.
/// \return TWBitcoinAddress pointer or nullptr if string is invalid.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithString(TWString *_Nonnull string);

/// Initializes an address from raw data.
///
/// \param data Raw data to initialize the address from. Must be deleted with TWBitcoinAddressDelete after use.
/// \return TWBitcoinAddress pointer or nullptr if data is invalid.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithData(TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
///
/// \param publicKey Public key to initialize the address from.
/// \param prefix Prefix byte (p2pkh, p2sh, etc).
/// \return TWBitcoinAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinAddress *_Nullable TWBitcoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

/// Deletes a legacy Bitcoin address.
///
/// \param address Address to delete.
TW_EXPORT_METHOD
void TWBitcoinAddressDelete(struct TWBitcoinAddress *_Nonnull address);

/// Returns the address in Base58 string representation.
///
/// \param address Address to get the string representation of.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBitcoinAddressDescription(struct TWBitcoinAddress *_Nonnull address);

/// Returns the address prefix.
///
/// \param address Address to get the prefix of.
TW_EXPORT_PROPERTY
uint8_t TWBitcoinAddressPrefix(struct TWBitcoinAddress *_Nonnull address);

/// Returns the key hash data.
///
/// \param address Address to get the keyhash data of.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWBitcoinAddressKeyhash(struct TWBitcoinAddress *_Nonnull address);

TW_EXTERN_C_END
