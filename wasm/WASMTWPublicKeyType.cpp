// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "TrustWalletCore/TWPublicKeyType.h"
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(wallet_publickeytype_module) {
    enum_<TWPublicKeyType>("TWPublicKeyType")
        .value("TWPublicKeyTypeSECP256k1", TWPublicKeyType::TWPublicKeyTypeSECP256k1)
        .value("TWPublicKeyTypeSECP256k1Extended",
               TWPublicKeyType::TWPublicKeyTypeSECP256k1Extended)
        .value("TWPublicKeyTypeNIST256p1", TWPublicKeyType::TWPublicKeyTypeNIST256p1)
        .value("TWPublicKeyTypeNIST256p1Extended",
               TWPublicKeyType::TWPublicKeyTypeNIST256p1Extended)
        .value("TWPublicKeyTypeED25519", TWPublicKeyType::TWPublicKeyTypeED25519)
        .value("TWPublicKeyTypeED25519Blake2b", TWPublicKeyType::TWPublicKeyTypeED25519Blake2b)
        .value("TWPublicKeyTypeCURVE25519", TWPublicKeyType::TWPublicKeyTypeCURVE25519)
        .value("TWPublicKeyTypeED25519Extended", TWPublicKeyType::TWPublicKeyTypeED25519Extended);
}
