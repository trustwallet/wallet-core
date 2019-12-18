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

#include <TrustWalletCore/TWNULSAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "NULSAddress.h"

using namespace Napi;

External<TWNULSAddress> Js_wallet_core_napi_NULSAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWNULSAddress *instance = TWNULSAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNULSAddress>::New(env, instance, [](Env _env, void* data) {
        TWNULSAddressDelete(reinterpret_cast<TWNULSAddress *>(data));
    });
}

External<TWNULSAddress> Js_wallet_core_napi_NULSAddress_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWNULSAddress *instance = TWNULSAddressCreateWithData(dataData);
    TWDataDelete(dataData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNULSAddress>::New(env, instance, [](Env _env, void* data) {
        TWNULSAddressDelete(reinterpret_cast<TWNULSAddress *>(data));
    });
}

External<TWNULSAddress> Js_wallet_core_napi_NULSAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWNULSAddress *instance = TWNULSAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWNULSAddress>::New(env, instance, [](Env _env, void* data) {
        TWNULSAddressDelete(reinterpret_cast<TWNULSAddress *>(data));
    });
}

void Js_wallet_core_napi_NULSAddress_delete(const CallbackInfo& info) {
    TWNULSAddressDelete(info[0].As<External<TWNULSAddress>>().Data());
}

Boolean Js_wallet_core_napi_NULSAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNULSAddress *lhsInstance = info[0].As<External<TWNULSAddress>>().Data();
    struct TWNULSAddress *rhsInstance = info[1].As<External<TWNULSAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWNULSAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_NULSAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWNULSAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_NULSAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNULSAddress *instance = info[0].As<External<TWNULSAddress>>().Data();

    TWString* twResult = TWNULSAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NULSAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNULSAddress *instance = info[0].As<External<TWNULSAddress>>().Data();

    TWData* twResult = TWNULSAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

