// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWPrivateKey.h"
#include "TWPublicKey.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// TON wallet operations.
TW_EXPORT_CLASS
struct TWTONWallet;

/// Determines whether the English mnemonic and passphrase are valid.
///
/// \param mnemonic Non-null english mnemonic
/// \param passphrase Nullable optional passphrase
/// \note passphrase can be null or empty string if no passphrase required
/// \return whether the mnemonic and passphrase are valid (valid checksum)
TW_EXPORT_STATIC_METHOD
bool TWTONWalletIsValidMnemonic(TWString* _Nonnull mnemonic, TWString* _Nullable passphrase);

/// Creates a \TONWallet from a valid TON mnemonic and passphrase.
///
/// \param mnemonic Non-null english mnemonic
/// \param passphrase Nullable optional passphrase
/// \note Null is returned on invalid mnemonic and passphrase
/// \note passphrase can be null or empty string if no passphrase required
/// \return Nullable TWTONWallet
TW_EXPORT_STATIC_METHOD
struct TWTONWallet* _Nullable TWTONWalletCreateWithMnemonic(TWString* _Nonnull mnemonic, TWString* _Nullable passphrase);

/// Delete the given \TONWallet
///
/// \param wallet Non-null pointer to private key
TW_EXPORT_METHOD
void TWTONWalletDelete(struct TWTONWallet* _Nonnull wallet);

/// Generates Ed25519 private key associated with the wallet.
///
/// \param wallet non-null TWTONWallet
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \return The Ed25519 private key
TW_EXPORT_METHOD
struct TWPrivateKey* _Nonnull TWTONWalletGetKey(struct TWTONWallet* _Nonnull wallet);

/// Constructs a TON Wallet V4R2 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
///
/// \param publicKey wallet's public key.
/// \param workchain TON workchain to which the wallet belongs. Usually, base chain is used (0).
/// \param walletId wallet's ID allows to create multiple wallets for the same private key.
/// \return Pointer to a base64 encoded Bag Of Cells (BoC) StateInit. Null if invalid public key provided.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONWalletBuildV4R2StateInit(struct TWPublicKey* _Nonnull publicKey, int32_t workchain, int32_t walletId);

TW_EXTERN_C_END
