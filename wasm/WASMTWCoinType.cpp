// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWCoinType.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(wallet_cointype_module) {
    enum_<TWCoinType>("TWCoinType")
        .value("TWCoinTypeAeternity", TWCoinType::TWCoinTypeAeternity)
        .value("TWCoinTypeAion", TWCoinType::TWCoinTypeAion)
        .value("TWCoinTypeBinance", TWCoinType::TWCoinTypeBinance)
        .value("TWCoinTypeBitcoin", TWCoinType::TWCoinTypeBitcoin)
        .value("TWCoinTypeBitcoinCash", TWCoinType::TWCoinTypeBitcoinCash)
        .value("TWCoinTypeBitcoinGold", TWCoinType::TWCoinTypeBitcoinGold)
        .value("TWCoinTypeCallisto", TWCoinType::TWCoinTypeCallisto)
        .value("TWCoinTypeCardano", TWCoinType::TWCoinTypeCardano)
        .value("TWCoinTypeCosmos", TWCoinType::TWCoinTypeCosmos)
        .value("TWCoinTypeDash", TWCoinType::TWCoinTypeDash)
        .value("TWCoinTypeDecred", TWCoinType::TWCoinTypeDecred)
        .value("TWCoinTypeDigiByte", TWCoinType::TWCoinTypeDigiByte)
        .value("TWCoinTypeDogecoin", TWCoinType::TWCoinTypeDogecoin)
        .value("TWCoinTypeEOS", TWCoinType::TWCoinTypeEOS)
        .value("TWCoinTypeEthereum", TWCoinType::TWCoinTypeEthereum)
        .value("TWCoinTypeEthereumClassic", TWCoinType::TWCoinTypeEthereumClassic)
        .value("TWCoinTypeFIO", TWCoinType::TWCoinTypeFIO)
        .value("TWCoinTypeGoChain", TWCoinType::TWCoinTypeGoChain)
        .value("TWCoinTypeGroestlcoin", TWCoinType::TWCoinTypeGroestlcoin)
        .value("TWCoinTypeICON", TWCoinType::TWCoinTypeICON)
        .value("TWCoinTypeIoTeX", TWCoinType::TWCoinTypeIoTeX)
        .value("TWCoinTypeKava", TWCoinType::TWCoinTypeKava)
        .value("TWCoinTypeKin", TWCoinType::TWCoinTypeKin)
        .value("TWCoinTypeLitecoin", TWCoinType::TWCoinTypeLitecoin)
        .value("TWCoinTypeMonacoin", TWCoinType::TWCoinTypeMonacoin)
        .value("TWCoinTypeNebulas", TWCoinType::TWCoinTypeNebulas)
        .value("TWCoinTypeNULS", TWCoinType::TWCoinTypeNULS)
        .value("TWCoinTypeNano", TWCoinType::TWCoinTypeNano)
        .value("TWCoinTypeNEAR", TWCoinType::TWCoinTypeNEAR)
        .value("TWCoinTypeNimiq", TWCoinType::TWCoinTypeNimiq)
        .value("TWCoinTypeOntology", TWCoinType::TWCoinTypeOntology)
        .value("TWCoinTypePOANetwork", TWCoinType::TWCoinTypePOANetwork)
        .value("TWCoinTypeQtum", TWCoinType::TWCoinTypeQtum)
        .value("TWCoinTypeXRP", TWCoinType::TWCoinTypeXRP)
        .value("TWCoinTypeSolana", TWCoinType::TWCoinTypeSolana)
        .value("TWCoinTypeStellar", TWCoinType::TWCoinTypeStellar)
        .value("TWCoinTypeTezos", TWCoinType::TWCoinTypeTezos)
        .value("TWCoinTypeTheta", TWCoinType::TWCoinTypeTheta)
        .value("TWCoinTypeThunderToken", TWCoinType::TWCoinTypeThunderToken)
        .value("TWCoinTypeNEO", TWCoinType::TWCoinTypeNEO)
        .value("TWCoinTypeTomoChain", TWCoinType::TWCoinTypeTomoChain)
        .value("TWCoinTypeTron", TWCoinType::TWCoinTypeTron)
        .value("TWCoinTypeVeChain", TWCoinType::TWCoinTypeVeChain)
        .value("TWCoinTypeViacoin", TWCoinType::TWCoinTypeViacoin)
        .value("TWCoinTypeWanchain", TWCoinType::TWCoinTypeWanchain)
        .value("TWCoinTypeZcash", TWCoinType::TWCoinTypeZcash)
        .value("TWCoinTypeFiro", TWCoinType::TWCoinTypeFiro)
        .value("TWCoinTypeZilliqa", TWCoinType::TWCoinTypeZilliqa)
        .value("TWCoinTypeZelcash", TWCoinType::TWCoinTypeZelcash)
        .value("TWCoinTypeRavencoin", TWCoinType::TWCoinTypeRavencoin)
        .value("TWCoinTypeWaves", TWCoinType::TWCoinTypeWaves)
        .value("TWCoinTypeTerra", TWCoinType::TWCoinTypeTerra)
        .value("TWCoinTypeHarmony", TWCoinType::TWCoinTypeHarmony)
        .value("TWCoinTypeAlgorand", TWCoinType::TWCoinTypeAlgorand)
        .value("TWCoinTypeKusama", TWCoinType::TWCoinTypeKusama)
        .value("TWCoinTypePolkadot", TWCoinType::TWCoinTypePolkadot)
        .value("TWCoinTypeFilecoin", TWCoinType::TWCoinTypeFilecoin)
        .value("TWCoinTypeElrond", TWCoinType::TWCoinTypeElrond)
        .value("TWCoinTypeBandChain", TWCoinType::TWCoinTypeBandChain)
        .value("TWCoinTypeSmartChainLegacy", TWCoinType::TWCoinTypeSmartChainLegacy)
        .value("TWCoinTypeSmartChain", TWCoinType::TWCoinTypeSmartChain)
        .value("TWCoinTypeOasis", TWCoinType::TWCoinTypeOasis)
        .value("TWCoinTypePolygon", TWCoinType::TWCoinTypePolygon)
        .value("TWCoinTypeTHORChain", TWCoinType::TWCoinTypeTHORChain)
        .value("TWCoinTypeOptimism", TWCoinType::TWCoinTypeOptimism)
        .value("TWCoinTypeECOChain", TWCoinType::TWCoinTypeECOChain)
        .value("TWCoinTypeAvalancheCChain", TWCoinType::TWCoinTypeAvalancheCChain)
        .value("TWCoinTypeXDai", TWCoinType::TWCoinTypeXDai)
        .value("TWCoinTypeFantom", TWCoinType::TWCoinTypeFantom)
        .value("TWCoinTypeCryptoOrg", TWCoinType::TWCoinTypeCryptoOrg)
        .value("TWCoinTypeCelo", TWCoinType::TWCoinTypeCelo)
        .value("TWCoinTypeRonin", TWCoinType::TWCoinTypeRonin)
        .value("TWCoinTypeOsmosis", TWCoinType::TWCoinTypeOsmosis)
        .value("TWCoinTypeCronosChain", TWCoinType::TWCoinTypeCronosChain)
        .value("TWCoinTypeSmartBitcoinCash", TWCoinType::TWCoinTypeSmartBitcoinCash);

    function("TWCoinTypeCurve", &TWCoinTypeCurve);
}
