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

EMSCRIPTEN_BINDINGS(wallet_purpose_module) {
    enum_<TWPurpose>("TWPurpose")
        .value("TWPurposeBIP44", TWPurpose::TWPurposeBIP44)
        .value("TWPurposeBIP49", TWPurpose::TWPurposeBIP49)
        .value("TWPurposeBIP84", TWPurpose::TWPurposeBIP84)
        .value("TWPurposeBIP1852", TWPurpose::TWPurposeBIP1852);
}
