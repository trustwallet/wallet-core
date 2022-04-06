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

EMSCRIPTEN_BINDINGS(wallet_blockchain_module) {
    enum_<TWBlockchain>("TWBlockchain")
        .value("TWBlockchainBitcoin", TWBlockchain::TWBlockchainBitcoin)
        .value("TWBlockchainEthereum", TWBlockchain::TWBlockchainEthereum)
        .value("TWBlockchainVechain", TWBlockchain::TWBlockchainVechain)
        .value("TWBlockchainTron", TWBlockchain::TWBlockchainTron)
        .value("TWBlockchainIcon", TWBlockchain::TWBlockchainIcon)
        .value("TWBlockchainBinance", TWBlockchain::TWBlockchainBinance)
        .value("TWBlockchainRipple", TWBlockchain::TWBlockchainRipple)
        .value("TWBlockchainTezos", TWBlockchain::TWBlockchainTezos)
        .value("TWBlockchainNimiq", TWBlockchain::TWBlockchainNimiq)
        .value("TWBlockchainStellar", TWBlockchain::TWBlockchainStellar)
        .value("TWBlockchainAion", TWBlockchain::TWBlockchainAion)
        .value("TWBlockchainCosmos", TWBlockchain::TWBlockchainCosmos)
        .value("TWBlockchainTheta", TWBlockchain::TWBlockchainTheta)
        .value("TWBlockchainOntology", TWBlockchain::TWBlockchainOntology)
        .value("TWBlockchainZilliqa", TWBlockchain::TWBlockchainZilliqa)
        .value("TWBlockchainIoTeX", TWBlockchain::TWBlockchainIoTeX)
        .value("TWBlockchainEOS", TWBlockchain::TWBlockchainEOS)
        .value("TWBlockchainNULS", TWBlockchain::TWBlockchainNULS)
        .value("TWBlockchainWaves", TWBlockchain::TWBlockchainWaves)
        .value("TWBlockchainAeternity", TWBlockchain::TWBlockchainAeternity)
        .value("TWBlockchainNebulas", TWBlockchain::TWBlockchainNebulas)
        .value("TWBlockchainFIO", TWBlockchain::TWBlockchainFIO)
        .value("TWBlockchainSolana", TWBlockchain::TWBlockchainSolana)
        .value("TWBlockchainHarmony", TWBlockchain::TWBlockchainHarmony)
        .value("TWBlockchainNEAR", TWBlockchain::TWBlockchainNEAR)
        .value("TWBlockchainAlgorand", TWBlockchain::TWBlockchainAlgorand)
        .value("TWBlockchainPolkadot", TWBlockchain::TWBlockchainPolkadot)
        .value("TWBlockchainCardano", TWBlockchain::TWBlockchainCardano)
        .value("TWBlockchainNEO", TWBlockchain::TWBlockchainNEO)
        .value("TWBlockchainFilecoin", TWBlockchain::TWBlockchainFilecoin)
        .value("TWBlockchainElrondNetwork", TWBlockchain::TWBlockchainElrondNetwork)
        .value("TWBlockchainOasisNetwork", TWBlockchain::TWBlockchainOasisNetwork);
}
