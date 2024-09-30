// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWFilecoinAddressType.h"
#include "TWFiroAddressType.h"
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

/// Determines if the string is a valid Any address with the given hrp.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \param hrp explicit given hrp of the given address.
/// \return bool indicating if the address is valid.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressIsValidBech32(TWString* _Nonnull string, enum TWCoinType coin, TWString* _Nonnull hrp);

/// Determines if the string is a valid Any address with the given SS58 network prefix.
///
/// \param string address to validate.
/// \param coin coin type of the address.
/// \param ss58Prefix ss58Prefix of the given address.
/// \return bool indicating if the address is valid.
TW_EXPORT_STATIC_METHOD
bool TWAnyAddressIsValidSS58(TWString* _Nonnull string, enum TWCoinType coin, uint32_t ss58Prefix);

/// Creates an address from a string representation and a coin type. Must be deleted with TWAnyAddressDelete after use.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \return TWAnyAddress pointer or nullptr if address and coin are invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nullable TWAnyAddressCreateWithString(TWString* _Nonnull string, enum TWCoinType coin);

/// Creates an bech32 address from a string representation, a coin type and the given hrp. Must be deleted with TWAnyAddressDelete after use.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \param hrp hrp of the address.
/// \return TWAnyAddress pointer or nullptr if address and coin are invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nullable TWAnyAddressCreateBech32(TWString* _Nonnull string, enum TWCoinType coin, TWString* _Nonnull hrp);

/// Creates an SS58 address from a string representation, a coin type and the given ss58Prefix. Must be deleted with TWAnyAddressDelete after use.
///
/// \param string address to create.
/// \param coin coin type of the address.
/// \param ss58Prefix ss58Prefix of the SS58 address.
/// \return TWAnyAddress pointer or nullptr if address and coin are invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nullable TWAnyAddressCreateSS58(TWString* _Nonnull string, enum TWCoinType coin, uint32_t ss58Prefix);


/// Creates an address from a public key.
///
/// \param publicKey derivates the address from the public key.
/// \param coin coin type of the address.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKey(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin);

/// Creates an address from a public key and derivation option.
///
/// \param publicKey derivates the address from the public key.
/// \param coin coin type of the address.
/// \param derivation the custom derivation to use.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKeyDerivation(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, enum TWDerivation derivation);

/// Creates an bech32 address from a public key and a given hrp.
///
/// \param publicKey derivates the address from the public key.
/// \param coin coin type of the address.
/// \param hrp hrp of the address.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateBech32WithPublicKey(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, TWString* _Nonnull hrp);

/// Creates an SS58 address from a public key and a given ss58Prefix.
///
/// \param publicKey derivates the address from the public key.
/// \param coin coin type of the address.
/// \param ss58Prefix ss58Prefix of the SS58 address.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateSS58WithPublicKey(struct TWPublicKey* _Nonnull publicKey, enum TWCoinType coin, uint32_t ss58Prefix);

/// Creates a Filecoin address from a public key and a given address type.
///
/// \param publicKey derivates the address from the public key.
/// \param filecoinAddressType Filecoin address type.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKeyFilecoinAddressType(struct TWPublicKey* _Nonnull publicKey, enum TWFilecoinAddressType filecoinAddressType);

/// Creates a Firo address from a public key and a given address type.
///
/// \param publicKey derivates the address from the public key.
/// \param firoAddressType Firo address type.
/// \return TWAnyAddress pointer or nullptr if public key is invalid.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWAnyAddressCreateWithPublicKeyFiroAddressType(struct TWPublicKey* _Nonnull publicKey, enum TWFiroAddressType firoAddressType);

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
