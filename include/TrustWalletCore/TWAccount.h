// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWDerivation.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Account for a particular coin within a wallet.
TW_EXPORT_CLASS
struct TWAccount;

/// Create a new account with the given parameters
///
/// \param address Account public address
/// \param coin Account coin type
/// \param derivation Account derivation type
/// \param derivationPath Account derivation path
/// \param publicKey Account pubkey
/// \param extendedPublicKey Account extended pubkey
/// \note call @TWAccountDelete to delete the account / deallocate the memory
/// \return A pointer to the created account
TW_EXPORT_STATIC_METHOD
struct TWAccount* _Nonnull TWAccountCreate(TWString* _Nonnull address, enum TWCoinType coin, enum TWDerivation derivation, TWString* _Nonnull derivationPath, TWString* _Nonnull publicKey, TWString* _Nonnull extendedPublicKey);

/// Delete the given account
///
/// \param account Account to be deleted
TW_EXPORT_METHOD
void TWAccountDelete(struct TWAccount* _Nonnull account);

/// Retrieve the public address of the given account
///
/// \param account Pointer to the account
/// \return The account public address
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountAddress(struct TWAccount* _Nonnull account);

/// Retrieve the Derivation type of the given account
///
/// \param account Pointer to the account
/// \return The account derivation type
TW_EXPORT_PROPERTY
enum TWDerivation TWAccountDerivation(struct TWAccount* _Nonnull account);

/// Retrieve the Derivation path of the given account
///
/// \param account Pointer to the account
/// \return The account derivation type
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountDerivationPath(struct TWAccount* _Nonnull account);

/// Retrieve the Public key of the given account
///
/// \param account Pointer to the account
/// \return The account public key
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountPublicKey(struct TWAccount* _Nonnull account);

/// Retrieve the Extended Public key of the given account
///
/// \param account Pointer to the account
/// \return The account extended public key
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountExtendedPublicKey(struct TWAccount* _Nonnull account);

/// Retrieve the Coin type of the given account
///
/// \param account Pointer to the account
/// \return The account coin type
TW_EXPORT_PROPERTY
enum TWCoinType TWAccountCoin(struct TWAccount* _Nonnull account);

TW_EXTERN_C_END
