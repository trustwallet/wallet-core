// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWPurpose.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a BIP44 DerivationPath in C++.
TW_EXPORT_CLASS
struct TWDerivationPath;

/// Creates a new DerivationPath with a purpose, coin, account, change and address.
/// Must be deleted with TWDerivationPathDelete after use.
///
/// \param purpose The purpose of the Path.
/// \param coin The coin type of the Path.
/// \param account The derivation of the Path.
/// \param change The derivation path of the Path.
/// \param address hex encoded public key.
/// \return A new DerivationPath.
TW_EXPORT_STATIC_METHOD
struct TWDerivationPath* _Nonnull TWDerivationPathCreate(enum TWPurpose purpose, uint32_t coin, uint32_t account, uint32_t change, uint32_t address);

/// Creates a new DerivationPath with a string
///
/// \param string The string of the Path.
/// \return A new DerivationPath or null if string is invalid.
TW_EXPORT_STATIC_METHOD
struct TWDerivationPath* _Nullable TWDerivationPathCreateWithString(TWString* _Nonnull string);

/// Deletes a DerivationPath.
///
/// \param path DerivationPath to delete.
TW_EXPORT_METHOD
void TWDerivationPathDelete(struct TWDerivationPath* _Nonnull path);

/// Returns the index component of a DerivationPath.
///
/// \param path DerivationPath to get the index of.
/// \param index The index component of the DerivationPath.
/// \return DerivationPathIndex or null if index is invalid.
TW_EXPORT_METHOD
struct TWDerivationPathIndex* _Nullable TWDerivationPathIndexAt(struct TWDerivationPath* _Nonnull path, uint32_t index);

/// Returns the indices count of a DerivationPath.
///
/// \param path DerivationPath to get the indices count of.
/// \return The indices count of the DerivationPath.
TW_EXPORT_METHOD
uint32_t TWDerivationPathIndicesCount(struct TWDerivationPath* _Nonnull path);

/// Returns the purpose enum of a DerivationPath.
///
/// \param path DerivationPath to get the purpose of.
/// \return DerivationPathPurpose.
TW_EXPORT_PROPERTY
enum TWPurpose TWDerivationPathPurpose(struct TWDerivationPath* _Nonnull path);

/// Returns the coin value of a derivation path.
///
/// \param path DerivationPath to get the coin of.
/// \return The coin part of the DerivationPath.
TW_EXPORT_PROPERTY
uint32_t TWDerivationPathCoin(struct TWDerivationPath* _Nonnull path);

/// Returns the account value of a derivation path.
///
/// \param path DerivationPath to get the account of.
/// \return the account part of a derivation path.
TW_EXPORT_PROPERTY
uint32_t TWDerivationPathAccount(struct TWDerivationPath* _Nonnull path);

/// Returns the change value of a derivation path.
///
/// \param path DerivationPath to get the change of.
/// \return The change part of a derivation path.
TW_EXPORT_PROPERTY
uint32_t TWDerivationPathChange(struct TWDerivationPath* _Nonnull path);

/// Returns the address value of a derivation path.
///
/// \param path DerivationPath to get the address of.
/// \return The address part of the derivation path.
TW_EXPORT_PROPERTY
uint32_t TWDerivationPathAddress(struct TWDerivationPath* _Nonnull path);

/// Returns the string description of a derivation path.
///
/// \param path DerivationPath to get the address of.
/// \return The string description of the derivation path.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWDerivationPathDescription(struct TWDerivationPath* _Nonnull path);

TW_EXTERN_C_END
