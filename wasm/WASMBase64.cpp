// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Base64.h"
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(wallet_base64_module) {
    function("base64Encode", &TW::Base64::encode);
    function("base64Decode", &TW::Base64::decode);
    function("base64EncodeBase64Url", &TW::Base64::encodeBase64Url);
    function("base64DecodeBase64Url", &TW::Base64::decodeBase64Url);
}
