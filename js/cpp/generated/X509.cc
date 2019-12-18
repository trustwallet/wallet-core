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

#include <TrustWalletCore/TWX509.h>

#include "../TWNapi.h"
#include "X509.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_X509_encodeED25519PublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *publicKeyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWX509EncodeED25519PublicKey(publicKeyData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(publicKeyData);
    return result;
}

Uint8Array Js_wallet_core_napi_X509_decodeED25519PublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWX509DecodeED25519PublicKey(dataData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

