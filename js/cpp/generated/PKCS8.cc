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

#include <TrustWalletCore/TWPKCS8.h>

#include "../TWNapi.h"
#include "PKCS8.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_PKCS8_encodeED25519PrivateKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *PrivateKeyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWPKCS8EncodeED25519PrivateKey(PrivateKeyData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(PrivateKeyData);
    return result;
}

Uint8Array Js_wallet_core_napi_PKCS8_decodeED25519PrivateKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWPKCS8DecodeED25519PrivateKey(dataData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

