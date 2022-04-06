// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Polkadot/SS58Address.h"

#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_SS58Address_module) {
    class_<SS58Address>("SS58Address")
        .constructor<>()
        .constructor<const PublicKey&, byte>()
        .class_function("isValid", &SS58Address::isValid)
        .function("string", &SS58Address::string)
        .function("keyBytes", &SS58Address::keyBytes);
}
