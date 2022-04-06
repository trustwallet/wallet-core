// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Data.h"
#include "PrivateKey.h"
#include <emscripten/bind.h>
#include <tuple>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_private_module) {
    class_<PrivateKey>("PrivateKey")
        .constructor<const Data&>()
        .property("bytes", &PrivateKey::bytes)
        .class_function("isValid", select_overload<bool(const Data&)>(&PrivateKey::isValid))
        .class_function("isValid",
                        select_overload<bool(const Data&, TWCurve)>(&PrivateKey::isValid))
        .function("getPublicKey", &PrivateKey::getPublicKey)
        .function("sign", select_overload<Data(const Data&, TWCurve) const>(&PrivateKey::sign))
        .function("signAsDER", &PrivateKey::signAsDER)
        .function("signSchnorr", &PrivateKey::signSchnorr);
}
