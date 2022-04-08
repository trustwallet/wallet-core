// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "HDWallet.h"
#include <emscripten/bind.h>

using namespace TW;
using namespace emscripten;

EMSCRIPTEN_BINDINGS(wallet_module) {
    enum_<HDWallet::PrivateKeyType>("PrivateKeyType")
        .value("PrivateKeyTypeDefault32", HDWallet::PrivateKeyType::PrivateKeyTypeDefault32)
        .value("PrivateKeyTypeDoubleExtended", HDWallet::PrivateKeyType::PrivateKeyTypeDoubleExtended);

    class_<HDWallet>("HDWallet")
        .constructor<int, const std::string&>()
        .constructor<const std::string&, const std::string&, const bool>()
        .constructor<const HDWallet&>()
        .function("getSeed", &HDWallet::getSeed)
        .function("getPassphrase", &HDWallet::getPassphrase)
        .function("getMnemonic", &HDWallet::getMnemonic)
        .function("getEntropy", &HDWallet::getEntropy)
        .function("getMasterKey", &HDWallet::getMasterKey)
        .function("getMasterKeyExtension", &HDWallet::getMasterKeyExtension)
        .function("getKey", &HDWallet::getKey)
        .function("deriveAddress", &HDWallet::deriveAddress)
        .function("getExtendedPrivateKey", &HDWallet::getExtendedPrivateKey)
        .function("getExtendedPublicKey", &HDWallet::getExtendedPublicKey)
        .class_function("getPublicKeyFromExtended", &HDWallet::getPublicKeyFromExtended)
        .class_function("getPrivateKeyFromExtended", &HDWallet::getPrivateKeyFromExtended)
        .class_function("getPrivateKeyType", &HDWallet::getPrivateKeyType);
}
