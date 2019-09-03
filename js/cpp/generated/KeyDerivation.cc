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

#include <TrustWalletCore/TWKeyDerivation.h>

#include "../TWNapi.h"
#include "KeyDerivation.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_KeyDerivation_scrypt(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *passwordString = TWStringCreateWithJsString(info[0].As<String>());
    TWData *saltData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    uint64_t n = std::stoull((std::string) info[2].As<String>());
    uint32_t r = info[3].As<Number>().Int64Value();
    uint32_t p = info[4].As<Number>().Int64Value();
    size_t keyLength = std::stoull((std::string) info[5].As<String>());
    TWData* twResult = TWKeyDerivationScrypt(passwordString, saltData, n, r, p, keyLength);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(passwordString);
    TWDataDelete(saltData);
    return result;
}

Uint8Array Js_wallet_core_napi_KeyDerivation_pbkdf2256(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *passwordString = TWStringCreateWithJsString(info[0].As<String>());
    TWData *saltData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    uint32_t iterations = info[2].As<Number>().Int64Value();
    size_t keyLength = std::stoull((std::string) info[3].As<String>());
    TWData* twResult = TWKeyDerivationPBKDF2_256(passwordString, saltData, iterations, keyLength);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(passwordString);
    TWDataDelete(saltData);
    return result;
}

Uint8Array Js_wallet_core_napi_KeyDerivation_pbkdf2512(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *passwordString = TWStringCreateWithJsString(info[0].As<String>());
    TWData *saltData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    uint32_t iterations = info[2].As<Number>().Int64Value();
    size_t keyLength = std::stoull((std::string) info[3].As<String>());
    TWData* twResult = TWKeyDerivationPBKDF2_512(passwordString, saltData, iterations, keyLength);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(passwordString);
    TWDataDelete(saltData);
    return result;
}

