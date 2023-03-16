// Copyright © 2017-2023 Trust Wallet.
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

/// Represents an Account in C++ with address, coin type and public key info, an item within a keystore.
TW_EXPORT_CLASS
struct TWAccount;

/// Creates a new Account with an address, a coin type, derivation enum, derivationPath, publicKey,
/// and extendedPublicKey. Must be deleted with TWAccountDelete after use.
///
/// \param address The address of the Account.
/// \param coin The coin type of the Account.
/// \param derivation The derivation of the Account.
/// \param derivationPath The derivation path of the Account.
/// \param publicKey hex encoded public key.
/// \param extendedPublicKey Base58 encoded extended public key.
/// \return A new Account.
TW_EXPORT_STATIC_METHOD
struct TWAccount* _Nonnull TWAccountCreate(TWString* _Nonnull address,
                                           enum TWCoinType coin,
                                           enum TWDerivation derivation,
                                           TWString* _Nonnull derivationPath,
                                           TWString* _Nonnull publicKey,
                                           TWString* _Nonnull extendedPublicKey);
/// Deletes an account.
///
/// \param account Account to delete.
TW_EXPORT_METHOD
void TWAccountDelete(struct TWAccount *_Nonnull account);

/// Returns the address of an account.
///
/// \param account Account to get the address of.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWAccountAddress(struct TWAccount *_Nonnull account);

/// Return CoinType enum of an account.
///
/// \param account Account to get the coin type of.
TW_EXPORT_PROPERTY
enum TWCoinType TWAccountCoin(struct TWAccount* _Nonnull account);

/// Returns the derivation enum of an account.
///
/// \param account Account to get the derivation enum of.
TW_EXPORT_PROPERTY
enum TWDerivation TWAccountDerivation(struct TWAccount *_Nonnull account);

/// Returns derivationPath of an account.
///
/// \param account Account to get the derivation path of.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWAccountDerivationPath(struct TWAccount *_Nonnull account);

/// Returns hex encoded publicKey of an account.
///
/// \param account Account to get the public key of.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountPublicKey(struct TWAccount* _Nonnull account);

/// Returns Base58 encoded extendedPublicKey of an account.
///
/// \param account Account to get the extended public key of.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWAccountExtendedPublicKey(struct TWAccount* _Nonnull account);

TW_EXTERN_C_END
