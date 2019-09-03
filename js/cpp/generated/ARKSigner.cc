// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <napi.h>
#include <string.h>

#include <TrustWalletCore/TWARKSigner.h>

#include "../TWNapi.h"
#include "ARKSigner.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_ARKSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    TWData* twResult = TWARKSignerSign(inputData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

