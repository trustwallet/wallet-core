// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWBlockchain.h"
#include "TWCurve.h"
#include "TWPrivateKey.h"
#include "TWPurpose.h"
#include "TWString.h"
#include "TWHDVersion.h"

TW_EXTERN_C_BEGIN

/// Coin type for Level 2 of BIP44.
///
/// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
TW_EXPORT_ENUM(uint32_t)
enum TWCoinType {
    TWCoinTypeAion = 425,
    TWCoinTypeBinance = 714,
    TWCoinTypeBitcoin = 0,
    TWCoinTypeBitcoinCash = 145,
    TWCoinTypeBravoCoin = 282,
    TWCoinTypeCallisto = 820,
    TWCoinTypeCosmos = 118,
    TWCoinTypeDash = 5,
    TWCoinTypeDecred = 42,
    TWCoinTypeDogecoin = 3,
    TWCoinTypeEllaism = 163,
    TWCoinTypeEOS = 194,
    TWCoinTypeEthereum = 60,
    TWCoinTypeEthereumClassic = 61,
    TWCoinTypeEthersocial = 31102,
    TWCoinTypeGoChain = 6060,
    TWCoinTypeGroestlcoin = 17,
    TWCoinTypeICON = 74,
    TWCoinTypeIOST = 291,
    TWCoinTypeIocoin = 295,
    TWCoinTypeIoTeX = 304,
    TWCoinTypeKin = 2017,
    TWCoinTypeLitecoin = 2,
    TWCoinTypeNULS = 8964,
    TWCoinTypeLux = 3003,
    TWCoinTypeNano = 165,
    TWCoinTypeNEO = 888,
    TWCoinTypeNimiq = 242,
    TWCoinTypeOntology = 1024,
    TWCoinTypePOANetwork = 178,
    TWCoinTypeQtum = 2301,
    TWCoinTypeXRP = 144,
    TWCoinTypeSteem = 135,
    TWCoinTypeStellar = 148,
    TWCoinTypeTezos = 1729,
    TWCoinTypeTheta = 500,
    TWCoinTypeThunderToken = 1001,
    TWCoinTypeTomoChain = 889,
    TWCoinTypeTron = 195,
    TWCoinTypeVeChain = 818,
    TWCoinTypeViacoin = 14,
    TWCoinTypeWanchain = 5718350,
    TWCoinTypeXDai = 700,
    TWCoinTypeZcash = 133,
    TWCoinTypeZcoin = 136,
    TWCoinTypeZilliqa = 313,
    TWCoinTypeSemux = 7562605,
    TWCoinTypeDEXON = 237,
    TWCoinTypeZelcash = 19167,
    TWCoinTypeARK = 111,
};

/// Returns the blockchain for a coin type.
TW_EXPORT_PROPERTY
enum TWBlockchain TWCoinTypeBlockchain(enum TWCoinType coin);

/// Returns the purpose for a coin type.
TW_EXPORT_PROPERTY
enum TWPurpose TWCoinTypePurpose(enum TWCoinType coin);

/// Returns the curve that should be used for a coin type.
TW_EXPORT_PROPERTY
enum TWCurve TWCoinTypeCurve(enum TWCoinType coin);

/// Returns the xpub HD version that should be used for a coin type.
TW_EXPORT_PROPERTY
enum TWHDVersion TWCoinTypeXpubVersion(enum TWCoinType coin);

/// Returns the xprv HD version that should be used for a coin type.
TW_EXPORT_PROPERTY
enum TWHDVersion TWCoinTypeXprvVersion(enum TWCoinType coin);

/// Validates an address string.
TW_EXPORT_METHOD
bool TWCoinTypeValidate(enum TWCoinType coin, TWString *_Nonnull address);

/// Returns the default derivation path for a particular coin.
TW_EXPORT_METHOD
TWString *_Nonnull TWCoinTypeDerivationPath(enum TWCoinType coin);

/// Derives the address for a particular coin from the private key.
TW_EXPORT_METHOD
TWString *_Nonnull TWCoinTypeDeriveAddress(enum TWCoinType coin, struct TWPrivateKey *_Nonnull privateKey);

/// Derives the address for a particular coin from the public key.
TW_EXPORT_METHOD
TWString *_Nonnull TWCoinTypeDeriveAddressFromPublicKey(enum TWCoinType coin, struct TWPublicKey *_Nonnull publicKey);

TW_EXTERN_C_END
