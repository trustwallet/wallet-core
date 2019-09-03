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

#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "PublicKey.h"

using namespace Napi;

External<TWPublicKey> Js_wallet_core_napi_PublicKey_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWPublicKeyType typeValue = (TWPublicKeyType)((int) info[1].As<Number>());
    struct TWPublicKey *instance = TWPublicKeyCreateWithData(dataData, typeValue);
    TWDataDelete(dataData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWPublicKey>::New(env, instance, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

void Js_wallet_core_napi_PublicKey_delete(const CallbackInfo& info) {
    TWPublicKeyDelete(info[0].As<External<TWPublicKey>>().Data());
}

Boolean Js_wallet_core_napi_PublicKey_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWPublicKeyType typeValue = (TWPublicKeyType)((int) info[1].As<Number>());
    Boolean resultValue = Boolean::New(env, TWPublicKeyIsValid(dataData, typeValue));
    TWDataDelete(dataData);
    return resultValue;
}

Value Js_wallet_core_napi_PublicKey_recover(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *signatureData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWData *messageData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    struct TWPublicKey *result = TWPublicKeyRecover(signatureData, messageData);
    TWDataDelete(signatureData);
    TWDataDelete(messageData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Boolean Js_wallet_core_napi_PublicKey_isCompressed(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    Boolean resultValue = Boolean::New(env, TWPublicKeyIsCompressed(instance));
    return resultValue;
}

Value Js_wallet_core_napi_PublicKey_compressed(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    struct TWPublicKey *result = TWPublicKeyCompressed(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Value Js_wallet_core_napi_PublicKey_uncompressed(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    struct TWPublicKey *result = TWPublicKeyUncompressed(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Uint8Array Js_wallet_core_napi_PublicKey_data(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    TWData* twResult = TWPublicKeyData(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_PublicKey_keyType(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    enum TWPublicKeyType result = TWPublicKeyKeyType(instance);
    return Number::New(env, (int) result);;
}

String Js_wallet_core_napi_PublicKey_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    TWString* twResult = TWPublicKeyDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Boolean Js_wallet_core_napi_PublicKey_verify(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    TWData *signatureData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *messageData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    Boolean resultValue = Boolean::New(env, TWPublicKeyVerify(instance, signatureData, messageData));
    TWDataDelete(signatureData);
    TWDataDelete(messageData);
    return resultValue;
}

Boolean Js_wallet_core_napi_PublicKey_verifySchnorr(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *instance = info[0].As<External<TWPublicKey>>().Data();

    TWData *signatureData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWData *messageData = TWDataCreateWithUint8Array(info[2].As<Uint8Array>());
    Boolean resultValue = Boolean::New(env, TWPublicKeyVerifySchnorr(instance, signatureData, messageData));
    TWDataDelete(signatureData);
    TWDataDelete(messageData);
    return resultValue;
}

