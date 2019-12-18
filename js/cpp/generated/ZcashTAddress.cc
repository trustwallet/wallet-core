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
#include <TrustWalletCore/TWZcashTAddress.h>

#include "../TWNapi.h"
#include "ZcashTAddress.h"

using namespace Napi;

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWZcashTAddress *instance = TWZcashTAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWZcashTAddress>::New(env, instance, [](Env _env, void* data) {
        TWZcashTAddressDelete(reinterpret_cast<TWZcashTAddress *>(data));
    });
}

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWZcashTAddress *instance = TWZcashTAddressCreateWithData(dataData);
    TWDataDelete(dataData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWZcashTAddress>::New(env, instance, [](Env _env, void* data) {
        TWZcashTAddressDelete(reinterpret_cast<TWZcashTAddress *>(data));
    });
}

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    uint8_t prefix = info[1].As<Number>().Int64Value();
    struct TWZcashTAddress *instance = TWZcashTAddressCreateWithPublicKey(publicKeyInstance, prefix);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWZcashTAddress>::New(env, instance, [](Env _env, void* data) {
        TWZcashTAddressDelete(reinterpret_cast<TWZcashTAddress *>(data));
    });
}

void Js_wallet_core_napi_ZcashTAddress_delete(const CallbackInfo& info) {
    TWZcashTAddressDelete(info[0].As<External<TWZcashTAddress>>().Data());
}

Boolean Js_wallet_core_napi_ZcashTAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZcashTAddress *lhsInstance = info[0].As<External<TWZcashTAddress>>().Data();
    struct TWZcashTAddress *rhsInstance = info[1].As<External<TWZcashTAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWZcashTAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_ZcashTAddress_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    Boolean resultValue = Boolean::New(env, TWZcashTAddressIsValid(dataData));
    TWDataDelete(dataData);
    return resultValue;
}

Boolean Js_wallet_core_napi_ZcashTAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWZcashTAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_ZcashTAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZcashTAddress *instance = info[0].As<External<TWZcashTAddress>>().Data();

    TWString* twResult = TWZcashTAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

