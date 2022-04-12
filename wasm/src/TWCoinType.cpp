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

EMSCRIPTEN_BINDINGS(WASM_TWCoinType) {
    enum_<TWCoinType>("CoinType")
        .value("Aeternity", TWCoinType::TWCoinTypeAeternity)
        .value("Aion", TWCoinType::TWCoinTypeAion)
        .value("Binance", TWCoinType::TWCoinTypeBinance)
        .value("Bitcoin", TWCoinType::TWCoinTypeBitcoin)
        .value("BitcoinCash", TWCoinType::TWCoinTypeBitcoinCash)
        .value("BitcoinGold", TWCoinType::TWCoinTypeBitcoinGold)
        .value("Callisto", TWCoinType::TWCoinTypeCallisto)
        .value("Cardano", TWCoinType::TWCoinTypeCardano)
        .value("Cosmos", TWCoinType::TWCoinTypeCosmos)
        .value("Dash", TWCoinType::TWCoinTypeDash)
        .value("Decred", TWCoinType::TWCoinTypeDecred)
        .value("DigiByte", TWCoinType::TWCoinTypeDigiByte)
        .value("Dogecoin", TWCoinType::TWCoinTypeDogecoin)
        .value("EOS", TWCoinType::TWCoinTypeEOS)
        .value("Ethereum", TWCoinType::TWCoinTypeEthereum)
        .value("EthereumClassic", TWCoinType::TWCoinTypeEthereumClassic)
        .value("FIO", TWCoinType::TWCoinTypeFIO)
        .value("GoChain", TWCoinType::TWCoinTypeGoChain)
        .value("Groestlcoin", TWCoinType::TWCoinTypeGroestlcoin)
        .value("ICON", TWCoinType::TWCoinTypeICON)
        .value("IoTeX", TWCoinType::TWCoinTypeIoTeX)
        .value("Kava", TWCoinType::TWCoinTypeKava)
        .value("Kin", TWCoinType::TWCoinTypeKin)
        .value("Litecoin", TWCoinType::TWCoinTypeLitecoin)
        .value("Monacoin", TWCoinType::TWCoinTypeMonacoin)
        .value("Nebulas", TWCoinType::TWCoinTypeNebulas)
        .value("NULS", TWCoinType::TWCoinTypeNULS)
        .value("Nano", TWCoinType::TWCoinTypeNano)
        .value("NEAR", TWCoinType::TWCoinTypeNEAR)
        .value("Nimiq", TWCoinType::TWCoinTypeNimiq)
        .value("Ontology", TWCoinType::TWCoinTypeOntology)
        .value("POANetwork", TWCoinType::TWCoinTypePOANetwork)
        .value("Qtum", TWCoinType::TWCoinTypeQtum)
        .value("XRP", TWCoinType::TWCoinTypeXRP)
        .value("Solana", TWCoinType::TWCoinTypeSolana)
        .value("Stellar", TWCoinType::TWCoinTypeStellar)
        .value("Tezos", TWCoinType::TWCoinTypeTezos)
        .value("Theta", TWCoinType::TWCoinTypeTheta)
        .value("ThunderToken", TWCoinType::TWCoinTypeThunderToken)
        .value("NEO", TWCoinType::TWCoinTypeNEO)
        .value("TomoChain", TWCoinType::TWCoinTypeTomoChain)
        .value("Tron", TWCoinType::TWCoinTypeTron)
        .value("VeChain", TWCoinType::TWCoinTypeVeChain)
        .value("Viacoin", TWCoinType::TWCoinTypeViacoin)
        .value("Wanchain", TWCoinType::TWCoinTypeWanchain)
        .value("Zcash", TWCoinType::TWCoinTypeZcash)
        .value("Firo", TWCoinType::TWCoinTypeFiro)
        .value("Zilliqa", TWCoinType::TWCoinTypeZilliqa)
        .value("Zelcash", TWCoinType::TWCoinTypeZelcash)
        .value("Ravencoin", TWCoinType::TWCoinTypeRavencoin)
        .value("Waves", TWCoinType::TWCoinTypeWaves)
        .value("Terra", TWCoinType::TWCoinTypeTerra)
        .value("Harmony", TWCoinType::TWCoinTypeHarmony)
        .value("Algorand", TWCoinType::TWCoinTypeAlgorand)
        .value("Kusama", TWCoinType::TWCoinTypeKusama)
        .value("Polkadot", TWCoinType::TWCoinTypePolkadot)
        .value("Filecoin", TWCoinType::TWCoinTypeFilecoin)
        .value("Elrond", TWCoinType::TWCoinTypeElrond)
        .value("BandChain", TWCoinType::TWCoinTypeBandChain)
        .value("SmartChainLegacy", TWCoinType::TWCoinTypeSmartChainLegacy)
        .value("SmartChain", TWCoinType::TWCoinTypeSmartChain)
        .value("Oasis", TWCoinType::TWCoinTypeOasis)
        .value("Polygon", TWCoinType::TWCoinTypePolygon)
        .value("THORChain", TWCoinType::TWCoinTypeTHORChain)
        .value("Optimism", TWCoinType::TWCoinTypeOptimism)
        .value("ECOChain", TWCoinType::TWCoinTypeECOChain)
        .value("AvalancheCChain", TWCoinType::TWCoinTypeAvalancheCChain)
        .value("XDai", TWCoinType::TWCoinTypeXDai)
        .value("Fantom", TWCoinType::TWCoinTypeFantom)
        .value("CryptoOrg", TWCoinType::TWCoinTypeCryptoOrg)
        .value("Celo", TWCoinType::TWCoinTypeCelo)
        .value("Ronin", TWCoinType::TWCoinTypeRonin)
        .value("Osmosis", TWCoinType::TWCoinTypeOsmosis)
        .value("CronosChain", TWCoinType::TWCoinTypeCronosChain)
        .value("SmartBitcoinCash", TWCoinType::TWCoinTypeSmartBitcoinCash);

    function("CoinTypeCurve", &TWCoinTypeCurve);
}
