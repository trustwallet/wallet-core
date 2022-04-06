// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <Mnemonic.h>
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_mnemonic_module) {
    class_<Mnemonic>("Mnemonic")
        .class_function("isValid", &Mnemonic::isValid)
        .class_function("isValidWord", &Mnemonic::isValidWord)
        .class_function("suggest", &Mnemonic::suggest);
}
