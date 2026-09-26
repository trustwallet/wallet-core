// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWCurve.h"
#include "TWData.h"
#include "TWDerivation.h"
#include "TWDerivationPath.h"
#include "TWHDVersion.h"
#include "TWDerivation.h"
#include "TWPrivateKey.h"
#include "TWPublicKey.h"
#include "TWPurpose.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Hierarchical Deterministic (HD) Wallet
TW_EXPORT_CLASS
struct TWHDWallet;

/// Creates a new HDWallet with a new random mnemonic with the provided strength in bits.
///
/// \param strength strength in bits
/// \param passphrase non-null passphrase
/// \note Null is returned on invalid strength
/// \note Returned object needs to be deleted with \TWHDWalletDelete
/// \return Nullable TWHDWallet
TW_EXPORT_STATIC_METHOD
struct TWHDWallet* _Nullable TWHDWalletCreate(int strength, TWString* _Nonnull passphrase);

/// Creates an HDWallet from a valid BIP39 English mnemonic and a passphrase.
///
/// \param mnemonic non-null Valid BIP39 mnemonic
/// \param passphrase  non-null passphrase
/// \note Null is returned on invalid mnemonic
/// \note Returned object needs to be deleted with \TWHDWalletDelete
/// \return Nullable TWHDWallet
TW_EXPORT_STATIC_METHOD
struct TWHDWallet* _Nullable TWHDWalletCreateWithMnemonic(TWString* _Nonnull mnemonic, TWString* _Nonnull passphrase);

/// Creates an HDWallet from a BIP39 mnemonic, a passphrase and validation flag.
///
/// \param mnemonic non-null Valid BIP39 mnemonic
/// \param passphrase  non-null passphrase
/// \param check validation flag
/// \note Null is returned on invalid mnemonic
/// \note Returned object needs to be deleted with \TWHDWalletDelete
/// \return Nullable TWHDWallet
TW_EXPORT_STATIC_METHOD
struct TWHDWallet* _Nullable TWHDWalletCreateWithMnemonicCheck(TWString* _Nonnull mnemonic, TWString* _Nonnull passphrase, bool check);

/// Creates an HDWallet from entropy (corresponding to a mnemonic).
///
/// \param entropy Non-null entropy data (corresponding to a mnemonic)
/// \param passphrase non-null passphrase
/// \note Null is returned on invalid input
/// \note Returned object needs to be deleted with \TWHDWalletDelete
/// \return Nullable TWHDWallet
TW_EXPORT_STATIC_METHOD
struct TWHDWallet* _Nullable TWHDWalletCreateWithEntropy(TWData* _Nonnull entropy, TWString* _Nonnull passphrase);

/// Deletes a wallet.
///
/// \param wallet non-null TWHDWallet
TW_EXPORT_METHOD
void TWHDWalletDelete(struct TWHDWallet* _Nonnull wallet);

/// Wallet seed.
///
/// \param wallet non-null TWHDWallet
/// \return The wallet seed as a Non-null block of data.
TW_EXPORT_PROPERTY
TWData* _Nonnull TWHDWalletSeed(struct TWHDWallet* _Nonnull wallet);

/// Wallet Mnemonic
///
/// \param wallet non-null TWHDWallet
/// \return The wallet mnemonic as a non-null TWString
TW_EXPORT_PROPERTY
TWString* _Nonnull TWHDWalletMnemonic(struct TWHDWallet* _Nonnull wallet);

/// Wallet entropy
///
/// \param wallet non-null TWHDWallet
/// \return The wallet entropy as a non-null block of data.
TW_EXPORT_PROPERTY
TWData* _Nonnull TWHDWalletEntropy(struct TWHDWallet* _Nonnull wallet);

/// Returns master key.
///
/// \param wallet non-null TWHDWallet
/// \param curve  a curve
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Nullable corresponding private key
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetMasterKey(struct TWHDWallet* _Nonnull wallet, enum TWCurve curve);

/// Generates the default private key for the specified coin, using default derivation.
///
/// \see TWHDWalletGetKey
/// \see TWHDWalletGetKeyDerivation
/// \param wallet non-null TWHDWallet
/// \param coin  a coin type
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return the default private key for the specified coin, or null on failure
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetKeyForCoin(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin);

/// Generates the default address for the specified coin (without exposing intermediary private key), default derivation.
///
/// \see TWHDWalletGetAddressDerivation
/// \param wallet non-null TWHDWallet
/// \param coin  a coin type
/// \note Null is returned if the address cannot be derived (e.g. empty mnemonic entropy)
/// \return the default address for the specified coin, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetAddressForCoin(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin);

/// Generates the default address for the specified coin and derivation (without exposing intermediary private key).
///
/// \see TWHDWalletGetAddressForCoin
/// \param wallet non-null TWHDWallet
/// \param coin  a coin type
/// \param derivation  a (custom) derivation to use
/// \note Null is returned if the address cannot be derived (e.g. empty mnemonic entropy)
/// \return the default address for the specified coin, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetAddressDerivation(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin, enum TWDerivation derivation);

/// Generates the private key for the specified derivation path.
///
/// \see TWHDWalletGetKeyForCoin
/// \see TWHDWalletGetKeyDerivation
/// \param wallet non-null TWHDWallet
/// \param coin a coin type
/// \param derivationPath  a non-null derivation path
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \return The private key for the specified derivation path/coin, or null if the path is invalid
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetKey(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin, TWString* _Nonnull derivationPath);

/// Generates the private key for the specified derivation.
///
/// \see TWHDWalletGetKey
/// \see TWHDWalletGetKeyForCoin
/// \param wallet non-null TWHDWallet
/// \param coin a coin type
/// \param derivation a (custom) derivation to use
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return The private key for the specified derivation path/coin, or null on failure
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetKeyDerivation(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin, enum TWDerivation derivation);

/// Generates the private key for the specified derivation path and curve.
///
/// \param wallet non-null TWHDWallet
/// \param curve a curve
/// \param derivationPath  a non-null derivation path
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \return The private key for the specified derivation path/curve, or null if the path is invalid
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetKeyByCurve(struct TWHDWallet* _Nonnull wallet, enum TWCurve curve, TWString* _Nonnull derivationPath);

/// Shortcut method to generate private key with the specified account/change/address (bip44 standard).
///
/// \see https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
///
/// \param wallet non-null TWHDWallet
/// \param coin a coin type
/// \param account valid bip44 account
/// \param change valid bip44 change
/// \param address valid bip44 address
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return The private key for the specified bip44 parameters, or null on failure
TW_EXPORT_METHOD
struct TWPrivateKey* _Nullable TWHDWalletGetDerivedKey(struct TWHDWallet* _Nonnull wallet, enum TWCoinType coin, uint32_t account, uint32_t change, uint32_t address);

/// Returns the extended private key (for default 0 account).
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param version hd version
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended private key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPrivateKey(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWHDVersion version);

/// Returns the extended public key (for default 0 account).
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param version hd version
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended public key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPublicKey(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWHDVersion version);

/// Returns the extended private key, for custom account.
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param derivation a derivation
/// \param version an hd version
/// \param account valid bip44 account
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended private key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPrivateKeyAccount(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWDerivation derivation, enum TWHDVersion version, uint32_t account);

/// Returns the extended public key, for custom account.
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param derivation a derivation
/// \param version an hd version
/// \param account valid bip44 account
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended public key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPublicKeyAccount(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWDerivation derivation, enum TWHDVersion version, uint32_t account);

/// Returns the extended private key (for default 0 account with derivation).
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param derivation a derivation
/// \param version an hd version
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended private key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPrivateKeyDerivation(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWDerivation derivation, enum TWHDVersion version);

/// Returns the extended public key (for default 0 account with derivation).
///
/// \param wallet non-null TWHDWallet
/// \param purpose a purpose
/// \param coin a coin type
/// \param derivation a derivation
/// \param version an hd version
/// \note Returned object needs to be deleted with \TWStringDelete
/// \note Null is returned if the key cannot be derived (e.g. empty mnemonic entropy)
/// \return Extended public key, or null on failure
TW_EXPORT_METHOD
TWString* _Nullable TWHDWalletGetExtendedPublicKeyDerivation(struct TWHDWallet* _Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWDerivation derivation, enum TWHDVersion version);

/// Computes the public key from an extended public key representation.
///
/// \param extended extended public key
/// \param coin a coin type
/// \param derivationPath a derivation path
/// \note Returned object needs to be deleted with \TWPublicKeyDelete
/// \return Nullable TWPublic key
TW_EXPORT_STATIC_METHOD
struct TWPublicKey* _Nullable TWHDWalletGetPublicKeyFromExtended(TWString* _Nonnull extended, enum TWCoinType coin, TWString* _Nonnull derivationPath);

TW_EXTERN_C_END
