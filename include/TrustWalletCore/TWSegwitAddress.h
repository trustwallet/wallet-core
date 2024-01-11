// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
///
/// \param lhs left non-null pointer to a Bech32 Address
/// \param rhs right non-null pointer to a Bech32 Address
/// \return true if both address are equal, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWSegwitAddressEqual(struct TWSegwitAddress *_Nonnull lhs, struct TWSegwitAddress *_Nonnull rhs);

/// Determines if the string is a valid Bech32 address.
///
/// \param string Non-null pointer to a Bech32 address as a string
/// \return true if the string is a valid Bech32 address, false otherwise.
TW_EXPORT_STATIC_METHOD
bool TWSegwitAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representation.
///
/// \param string Non-null pointer to a Bech32 address as a string
/// \note should be deleted with \TWSegwitAddressDelete
/// \return Pointer to a Bech32 address if the string is a valid Bech32 address, null pointer otherwise
TW_EXPORT_STATIC_METHOD
struct TWSegwitAddress *_Nullable TWSegwitAddressCreateWithString(TWString *_Nonnull string);

/// Creates a segwit-version-0 address from a public key and HRP prefix.
/// Taproot (v>=1) is not supported by this method.
///
/// \param hrp HRP of the utxo coin targeted
/// \param publicKey Non-null pointer to the public key of the targeted coin
/// \note should be deleted with \TWSegwitAddressDelete
/// \return Non-null pointer to the corresponding Segwit address
TW_EXPORT_STATIC_METHOD
struct TWSegwitAddress *_Nonnull TWSegwitAddressCreateWithPublicKey(enum TWHRP hrp, struct TWPublicKey *_Nonnull publicKey);

/// Delete the given Segwit address
///
/// \param address Non-null pointer to a Segwit address
TW_EXPORT_METHOD
void TWSegwitAddressDelete(struct TWSegwitAddress *_Nonnull address);

/// Returns the address string representation.
///
/// \param address Non-null pointer to a Segwit Address
/// \return Non-null pointer to the segwit address string representation
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSegwitAddressDescription(struct TWSegwitAddress *_Nonnull address);

/// Returns the human-readable part.
///
/// \param address Non-null pointer to a Segwit Address
/// \return the HRP part of the given address
TW_EXPORT_PROPERTY
enum TWHRP TWSegwitAddressHRP(struct TWSegwitAddress *_Nonnull address);

/// Returns the human-readable part.
///
/// \param address Non-null pointer to a Segwit Address
/// \return returns the witness version of the given segwit address
TW_EXPORT_PROPERTY
int TWSegwitAddressWitnessVersion(struct TWSegwitAddress *_Nonnull address);

/// Returns the witness program
///
/// \param address Non-null pointer to a Segwit Address
/// \return returns the witness data of the given segwit address as a non-null pointer block of data
TW_EXPORT_PROPERTY
TWData *_Nonnull TWSegwitAddressWitnessProgram(struct TWSegwitAddress *_Nonnull address);

TW_EXTERN_C_END
