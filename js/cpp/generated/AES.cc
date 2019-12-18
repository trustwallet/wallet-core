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

#include <TrustWalletCore/TWAES.h>

#include "../TWNapi.h"
#include "AES.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_AES_cbcencrypt(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData *dataData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *ivData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    TWData* twResult = TWAESCBCEncrypt(keyData, dataData, ivData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);
    return result;
}

Uint8Array Js_wallet_core_napi_AES_cbcdecrypt(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData *dataData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *ivData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    TWData* twResult = TWAESCBCDecrypt(keyData, dataData, ivData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);
    return result;
}

Uint8Array Js_wallet_core_napi_AES_ctrencrypt(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData *dataData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *ivData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    TWData* twResult = TWAESCTREncrypt(keyData, dataData, ivData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);
    return result;
}

Uint8Array Js_wallet_core_napi_AES_ctrdecrypt(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData *dataData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *ivData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    TWData* twResult = TWAESCTRDecrypt(keyData, dataData, ivData);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);
    return result;
}

