// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "../Address.h"

enum TWPurpose TWCoinTypePurpose(enum TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeBinance:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeCallisto:
    case TWCoinTypeDash:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeNimiq:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWPurposeBIP44;
    case TWCoinTypeBitcoin:
    case TWCoinTypeLitecoin:
        return TWPurposeBIP84;
    }
}

enum TWCurve TWCoinTypeCurve(enum TWCoinType coin) {
    switch(coin) {
    case TWCoinTypeBinance:
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeCallisto:
    case TWCoinTypeDash:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeICON:
    case TWCoinTypeLitecoin:
    case TWCoinTypePoa:
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        return TWCurveSECP256k1;
    case TWCoinTypeNimiq:
        return TWCurveEd25519;
    }
}

bool TWCoinTypeValidate(enum TWCoinType coin, TWString *_Nonnull address) {
    return TW::validate(*reinterpret_cast<const std::string*>(address), coin);
}
