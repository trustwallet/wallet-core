// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWHDVersion.h"
#include "TWPrivateKey.h"
#include "TWPublicKey.h"
#include "TWPurpose.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWHDWallet;

/// Determines if a mnemonic phrase is valid.
TW_EXPORT_STATIC_METHOD
bool TWHDWalletIsValid(TWString *_Nonnull mnemonic);

/// Creates a new random HDWallet with the provided strength in bits.
TW_EXPORT_STATIC_METHOD
struct TWHDWallet *_Nonnull TWHDWalletCreate(int strength, TWString *_Nonnull passphrase);

/// Creates an HDWallet from a mnemonic seed.
TW_EXPORT_STATIC_METHOD
struct TWHDWallet *_Nonnull TWHDWalletCreateWithMnemonic(TWString *_Nonnull mnemonic, TWString *_Nonnull passphrase);

/// Creates an HDWallet from a seed.
TW_EXPORT_STATIC_METHOD
struct TWHDWallet *_Nonnull TWHDWalletCreateWithData(TWData *_Nonnull data, TWString *_Nonnull passphrase);

/// Deletes a wallet.
TW_EXPORT_METHOD
void TWHDWalletDelete(struct TWHDWallet *_Nonnull wallet);

/// Wallet seed.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWHDWalletSeed(struct TWHDWallet *_Nonnull wallet);

// Wallet Mnemonic
TW_EXPORT_PROPERTY
TWString *_Nonnull TWHDWalletMnemonic(struct TWHDWallet *_Nonnull wallet);

/// Generates the private key at the specified derivation path.
TW_EXPORT_METHOD
struct TWPrivateKey *_Nonnull TWHDWalletGetKey(struct TWHDWallet *_Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, uint32_t account, uint32_t change, uint32_t address);

/// Returns the extended private key.
TW_EXPORT_METHOD
TWString *_Nonnull TWHDWalletGetExtendedPrivateKey(struct TWHDWallet *_Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWHDVersion version);

/// Returns the exteded public key.
TW_EXPORT_METHOD
TWString *_Nonnull TWHDWalletGetExtendedPublicKey(struct TWHDWallet *_Nonnull wallet, enum TWPurpose purpose, enum TWCoinType coin, enum TWHDVersion version);

/// Computes the public key from an exteded public key representation.
TW_EXPORT_STATIC_METHOD
struct TWPublicKey TWHDWalletGetPublicKeyFromExtended(TWString *_Nonnull extended, enum TWHDVersion versionPublic, enum TWHDVersion versionPrivate, uint32_t change, uint32_t address);

/// Generates an address from an exteded public key representation, coin type, and change and address indices.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWHDWalletGetAddressFromExtended(TWString *_Nonnull extended, enum TWCoinType coinType, uint32_t change, uint32_t address);

TW_EXTERN_C_END
