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

#include <TrustWalletCore/TWHash.h>

#include "../TWNapi.h"
#include "Hash.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_Hash_sha1(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA1(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha256(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA256(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha512(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA512(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_keccak256(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashKeccak256(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_keccak512(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashKeccak512(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha3256(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA3_256(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha3512(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA3_512(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_ripemd(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashRIPEMD(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_blake2b(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    size_t size = std::stoull((std::string) info[1].As<String>());
    TWData* twResult = TWHashBlake2b(dataData, size);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha256RIPEMD(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA256RIPEMD(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

Uint8Array Js_wallet_core_napi_Hash_sha256SHA256(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData* twResult = TWHashSHA256SHA256(dataData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(dataData);
    return result;
}

