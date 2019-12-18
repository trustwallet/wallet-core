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
#include <TrustWalletCore/TWZilliqaAddress.h>

#include "../TWNapi.h"
#include "ZilliqaAddress.h"

using namespace Napi;

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWZilliqaAddress *instance = TWZilliqaAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWZilliqaAddress>::New(env, instance, [](Env _env, void* data) {
        TWZilliqaAddressDelete(reinterpret_cast<TWZilliqaAddress *>(data));
    });
}

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWZilliqaAddress *instance = TWZilliqaAddressCreateWithKeyHash(keyHashData);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWZilliqaAddress>::New(env, instance, [](Env _env, void* data) {
        TWZilliqaAddressDelete(reinterpret_cast<TWZilliqaAddress *>(data));
    });
}

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWZilliqaAddress *instance = TWZilliqaAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWZilliqaAddress>::New(env, instance, [](Env _env, void* data) {
        TWZilliqaAddressDelete(reinterpret_cast<TWZilliqaAddress *>(data));
    });
}

void Js_wallet_core_napi_ZilliqaAddress_delete(const CallbackInfo& info) {
    TWZilliqaAddressDelete(info[0].As<External<TWZilliqaAddress>>().Data());
}

Boolean Js_wallet_core_napi_ZilliqaAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZilliqaAddress *lhsInstance = info[0].As<External<TWZilliqaAddress>>().Data();
    struct TWZilliqaAddress *rhsInstance = info[1].As<External<TWZilliqaAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWZilliqaAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_ZilliqaAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWZilliqaAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_ZilliqaAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZilliqaAddress *instance = info[0].As<External<TWZilliqaAddress>>().Data();

    TWString* twResult = TWZilliqaAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_ZilliqaAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZilliqaAddress *instance = info[0].As<External<TWZilliqaAddress>>().Data();

    TWString* twResult = TWZilliqaAddressKeyHash(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

