// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWPurpose.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(WASM_TWPurpose) {
    enum_<TWPurpose>("Purpose")
        .value("BIP44", TWPurpose::TWPurposeBIP44)
        .value("BIP49", TWPurpose::TWPurposeBIP49)
        .value("BIP84", TWPurpose::TWPurposeBIP84)
        .value("BIP1852", TWPurpose::TWPurposeBIP1852);
}
