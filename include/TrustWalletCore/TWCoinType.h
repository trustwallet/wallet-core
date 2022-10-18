// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWBlockchain.h"
#include "TWCoinTypeEnum.h"
#include "TWCurve.h"
#include "TWHDVersion.h"
#include "TWHRP.h"
#include "TWPrivateKey.h"
#include "TWPurpose.h"
#include "TWString.h"
#include "TWDerivation.h"

TW_EXTERN_C_BEGIN

/// Returns the blockchain for a coin type.
///
/// \param coin A coin type
/// \return blockchain associated to the given coin type
TW_EXPORT_PROPERTY
enum TWBlockchain TWCoinTypeBlockchain(enum TWCoinType coin);

/// Returns the purpose for a coin type.
///
/// \param coin A coin type
/// \return purpose associated to the given coin type
TW_EXPORT_PROPERTY
enum TWPurpose TWCoinTypePurpose(enum TWCoinType coin);

/// Returns the curve that should be used for a coin type.
///
/// \param coin A coin type
/// \return curve that should be used for the given coin type
TW_EXPORT_PROPERTY
enum TWCurve TWCoinTypeCurve(enum TWCoinType coin);

/// Returns the xpub HD version that should be used for a coin type.
///
/// \param coin A coin type
/// \return xpub HD version that should be used for the given coin type
TW_EXPORT_PROPERTY
enum TWHDVersion TWCoinTypeXpubVersion(enum TWCoinType coin);

/// Returns the xprv HD version that should be used for a coin type.
///
/// \param coin A coin type
/// \return the xprv HD version that should be used for the given coin type.
TW_EXPORT_PROPERTY
enum TWHDVersion TWCoinTypeXprvVersion(enum TWCoinType coin);

/// Validates an address string.
///
/// \param coin A coin type
/// \param address A public address
/// \return true if the address is a valid public address of the given coin, false otherwise.
TW_EXPORT_METHOD
bool TWCoinTypeValidate(enum TWCoinType coin, TWString* _Nonnull address);

/// Returns the default derivation path for a particular coin.
///
/// \param coin A coin type
/// \return the default derivation path for the given coin type.
TW_EXPORT_METHOD
TWString* _Nonnull TWCoinTypeDerivationPath(enum TWCoinType coin);

/// Returns the derivation path for a particular coin with the explicit given derivation.
///
/// \param coin A coin type
/// \param derivation A derivation type
/// \return the derivation path for the given coin with the explicit given derivation
TW_EXPORT_METHOD
TWString* _Nonnull TWCoinTypeDerivationPathWithDerivation(enum TWCoinType coin, enum TWDerivation derivation);

/// Derives the address for a particular coin from the private key.
///
/// \param coin A coin type
/// \param privateKey A valid private key
/// \return Derived address for the given coin from the private key.
TW_EXPORT_METHOD
TWString* _Nonnull TWCoinTypeDeriveAddress(enum TWCoinType coin,
                                           struct TWPrivateKey* _Nonnull privateKey);

/// Derives the address for a particular coin from the public key.
///
/// \param coin A coin type
/// \param publicKey A valid public key
/// \return Derived address for the given coin from the public key.
TW_EXPORT_METHOD
TWString* _Nonnull TWCoinTypeDeriveAddressFromPublicKey(enum TWCoinType coin,
                                                        struct TWPublicKey* _Nonnull publicKey);

/// HRP for this coin type
///
/// \param coin A coin type
/// \return HRP of the given coin type.
TW_EXPORT_PROPERTY
enum TWHRP TWCoinTypeHRP(enum TWCoinType coin);

/// P2PKH prefix for this coin type
///
/// \param coin A coin type
/// \return P2PKH prefix for the given coin type
TW_EXPORT_PROPERTY
uint8_t TWCoinTypeP2pkhPrefix(enum TWCoinType coin);

/// P2SH prefix for this coin type
///
/// \param coin A coin type
/// \return P2SH prefix for the given coin type
TW_EXPORT_PROPERTY
uint8_t TWCoinTypeP2shPrefix(enum TWCoinType coin);

/// Static prefix for this coin type
///
/// \param coin A coin type
/// \return Static prefix for the given coin type
TW_EXPORT_PROPERTY
uint8_t TWCoinTypeStaticPrefix(enum TWCoinType coin);

/// ChainID for this coin type.
///
/// \param coin A coin type
/// \return ChainID for the given coin type.
/// \note Caller must free returned object.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWCoinTypeChainId(enum TWCoinType coin);

/// SLIP-0044 id for this coin type
///
/// \param coin A coin type
/// \return SLIP-0044 id for the given coin type
TW_EXPORT_PROPERTY
uint32_t TWCoinTypeSlip44Id(enum TWCoinType coin);

/// public key type for this coin type
///
/// \param coin A coin type
/// \return public key type for the given coin type
TW_EXPORT_PROPERTY
enum TWPublicKeyType TWCoinTypePublicKeyType(enum TWCoinType coin);

TW_EXTERN_C_END
