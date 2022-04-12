// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWBlockchain.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(WASM_TWBlockchain) {
    enum_<TWBlockchain>("Blockchain")
        .value("Bitcoin", TWBlockchain::TWBlockchainBitcoin)
        .value("Ethereum", TWBlockchain::TWBlockchainEthereum)
        .value("Vechain", TWBlockchain::TWBlockchainVechain)
        .value("Tron", TWBlockchain::TWBlockchainTron)
        .value("Icon", TWBlockchain::TWBlockchainIcon)
        .value("Binance", TWBlockchain::TWBlockchainBinance)
        .value("Ripple", TWBlockchain::TWBlockchainRipple)
        .value("Tezos", TWBlockchain::TWBlockchainTezos)
        .value("Nimiq", TWBlockchain::TWBlockchainNimiq)
        .value("Stellar", TWBlockchain::TWBlockchainStellar)
        .value("Aion", TWBlockchain::TWBlockchainAion)
        .value("Cosmos", TWBlockchain::TWBlockchainCosmos)
        .value("Theta", TWBlockchain::TWBlockchainTheta)
        .value("Ontology", TWBlockchain::TWBlockchainOntology)
        .value("Zilliqa", TWBlockchain::TWBlockchainZilliqa)
        .value("IoTeX", TWBlockchain::TWBlockchainIoTeX)
        .value("EOS", TWBlockchain::TWBlockchainEOS)
        .value("NULS", TWBlockchain::TWBlockchainNULS)
        .value("Waves", TWBlockchain::TWBlockchainWaves)
        .value("Aeternity", TWBlockchain::TWBlockchainAeternity)
        .value("Nebulas", TWBlockchain::TWBlockchainNebulas)
        .value("FIO", TWBlockchain::TWBlockchainFIO)
        .value("Solana", TWBlockchain::TWBlockchainSolana)
        .value("Harmony", TWBlockchain::TWBlockchainHarmony)
        .value("NEAR", TWBlockchain::TWBlockchainNEAR)
        .value("Algorand", TWBlockchain::TWBlockchainAlgorand)
        .value("Polkadot", TWBlockchain::TWBlockchainPolkadot)
        .value("Cardano", TWBlockchain::TWBlockchainCardano)
        .value("NEO", TWBlockchain::TWBlockchainNEO)
        .value("Filecoin", TWBlockchain::TWBlockchainFilecoin)
        .value("ElrondNetwork", TWBlockchain::TWBlockchainElrondNetwork)
        .value("OasisNetwork", TWBlockchain::TWBlockchainOasisNetwork);
}
