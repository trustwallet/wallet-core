// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Data.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

Data gendata(const std::string& data) {
    return TW::data(data);
}

std::string dataToStr(const Data& data) {
    return std::string(data.begin(), data.end());
}

EMSCRIPTEN_BINDINGS(wallet_data_module) {
    class_<Data>("Data");
    function("gendata", &gendata);
    function("strToData", &gendata);
    function("dataToStr", &dataToStr);
}
