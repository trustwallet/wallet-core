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

#include <TrustWalletCore/TWBase58.h>

#include "../TWNapi.h"
#include "Base58.h"

using namespace Napi;

String Js_wallet_core_napi_Base58_encode(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWString* twResult = TWBase58Encode(dataData);
    String result = TWStringJsString(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

String Js_wallet_core_napi_Base58_encodeNoCheck(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWString* twResult = TWBase58EncodeNoCheck(dataData);
    String result = TWStringJsString(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Base58_decode(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    TWData* twResult = TWBase58Decode(stringString);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(stringString);
    return result;
}

Uint8Array Js_wallet_core_napi_Base58_decodeNoCheck(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    TWData* twResult = TWBase58DecodeNoCheck(stringString);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(stringString);
    return result;
}

