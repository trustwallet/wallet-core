// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "PublicKey.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_public_module) {
    class_<PublicKey>("PublicKey")
        .constructor<const Data&, enum TWPublicKeyType>()
        .class_function("isValid", &PublicKey::isValid)
        .property("bytes", &PublicKey::bytes)
        .function("verify", &PublicKey::verify)
        .function("verifySchnorr", &PublicKey::verifySchnorr)
        .function("isCompressed", &PublicKey::isCompressed)
        .function("compressed", &PublicKey::compressed)
        .function("extended", &PublicKey::extended)
        .function("hash", &PublicKey::hash)
        .class_function("recover", &PublicKey::recover)
        .function("isValidED25519", &PublicKey::isValidED25519);
}
