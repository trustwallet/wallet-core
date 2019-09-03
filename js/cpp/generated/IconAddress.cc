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

#include <TrustWalletCore/TWIconAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "IconAddress.h"

using namespace Napi;

External<TWIconAddress> Js_wallet_core_napi_IconAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWIconAddress *instance = TWIconAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWIconAddress>::New(env, instance, [](Env _env, void* data) {
        TWIconAddressDelete(reinterpret_cast<TWIconAddress *>(data));
    });
}

External<TWIconAddress> Js_wallet_core_napi_IconAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWIconAddressType typeValue = (TWIconAddressType)((int) info[1].As<Number>());
    struct TWIconAddress *instance = TWIconAddressCreateWithKeyHash(keyHashData, typeValue);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWIconAddress>::New(env, instance, [](Env _env, void* data) {
        TWIconAddressDelete(reinterpret_cast<TWIconAddress *>(data));
    });
}

External<TWIconAddress> Js_wallet_core_napi_IconAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    TWIconAddressType typeValue = (TWIconAddressType)((int) info[1].As<Number>());
    struct TWIconAddress *instance = TWIconAddressCreateWithPublicKey(publicKeyInstance, typeValue);
    return External<TWIconAddress>::New(env, instance, [](Env _env, void* data) {
        TWIconAddressDelete(reinterpret_cast<TWIconAddress *>(data));
    });
}

void Js_wallet_core_napi_IconAddress_delete(const CallbackInfo& info) {
    TWIconAddressDelete(info[0].As<External<TWIconAddress>>().Data());
}

Boolean Js_wallet_core_napi_IconAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIconAddress *lhsInstance = info[0].As<External<TWIconAddress>>().Data();
    struct TWIconAddress *rhsInstance = info[1].As<External<TWIconAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWIconAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_IconAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWIconAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_IconAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIconAddress *instance = info[0].As<External<TWIconAddress>>().Data();

    TWString* twResult = TWIconAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_IconAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIconAddress *instance = info[0].As<External<TWIconAddress>>().Data();

    TWData* twResult = TWIconAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

