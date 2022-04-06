// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "DerivationPath.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

std::string DerivationPathToString(DerivationPath dpath) {
    return dpath.string();
}

EMSCRIPTEN_BINDINGS(wallet_derivationpath_module) {
    class_<DerivationPathIndex>("DerivationPathIndex")
        .constructor<>()
        .constructor<uint32_t, bool>()
        .function("derivationIndex", &DerivationPathIndex::derivationIndex)
        .function("string", &DerivationPathIndex::string);

    class_<DerivationPath>("DerivationPath")
        .constructor<>()
        .constructor<TWPurpose, uint32_t, uint32_t, uint32_t, uint32_t>()
        .constructor<const std::string&>()
        .function("purpose", &DerivationPath::purpose)
        .function("setPurpose", &DerivationPath::setPurpose)
        .function("coin", &DerivationPath::coin)
        .function("setCoin", &DerivationPath::setCoin)
        .function("account", &DerivationPath::account)
        .function("setAccount", &DerivationPath::setAccount)
        .function("change", &DerivationPath::change)
        .function("setChange", &DerivationPath::setChange)
        .function("address", &DerivationPath::address)
        .function("setAddress", &DerivationPath::setAddress);

    function("derivationPathToString", &DerivationPathToString);
}
