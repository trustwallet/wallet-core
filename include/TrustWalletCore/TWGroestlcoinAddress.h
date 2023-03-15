// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a legacy Groestlcoin address.
TW_EXPORT_CLASS
struct TWGroestlcoinAddress;

/// Compares two addresses for equality.
///
/// \param lhs left Non-null GroestlCoin address to be compared
/// \param rhs right Non-null GroestlCoin address to be compared
/// \return true if both address are equal, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWGroestlcoinAddressEqual(struct TWGroestlcoinAddress *_Nonnull lhs, struct TWGroestlcoinAddress *_Nonnull rhs);

/// Determines if the string is a valid Groestlcoin address.
///
/// \param string Non-null string.
/// \return true if it's a valid address, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWGroestlcoinAddressIsValidString(TWString *_Nonnull string);

/// Create an address from a base58 string representation.
///
/// \param string Non-null string
/// \note Must be deleted with \TWGroestlcoinAddressDelete
/// \return Non-null GroestlcoinAddress
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinAddress *_Nullable TWGroestlcoinAddressCreateWithString(TWString *_Nonnull string);

/// Create an address from a public key and a prefix byte.
///
/// \param publicKey Non-null public key
/// \param prefix public key prefix
/// \note Must be deleted with \TWGroestlcoinAddressDelete
/// \return Non-null GroestlcoinAddress
TW_EXPORT_STATIC_METHOD
struct TWGroestlcoinAddress *_Nonnull TWGroestlcoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

/// Delete a Groestlcoin address
///
/// \param address Non-null GroestlcoinAddress
TW_EXPORT_METHOD
void TWGroestlcoinAddressDelete(struct TWGroestlcoinAddress *_Nonnull address);

/// Returns the address base58 string representation.
///
/// \param address Non-null GroestlcoinAddress
/// \return Address description as a non-null string
TW_EXPORT_PROPERTY
TWString *_Nonnull TWGroestlcoinAddressDescription(struct TWGroestlcoinAddress *_Nonnull address);

TW_EXTERN_C_END
