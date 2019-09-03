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

#include <TrustWalletCore/TWAeternityAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "AeternityAddress.h"

using namespace Napi;

External<TWAeternityAddress> Js_wallet_core_napi_AeternityAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWAeternityAddress *instance = TWAeternityAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWAeternityAddress>::New(env, instance, [](Env _env, void* data) {
        TWAeternityAddressDelete(reinterpret_cast<TWAeternityAddress *>(data));
    });
}

External<TWAeternityAddress> Js_wallet_core_napi_AeternityAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWAeternityAddress *instance = TWAeternityAddressCreateWithPublicKey(publicKeyInstance);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWAeternityAddress>::New(env, instance, [](Env _env, void* data) {
        TWAeternityAddressDelete(reinterpret_cast<TWAeternityAddress *>(data));
    });
}

void Js_wallet_core_napi_AeternityAddress_delete(const CallbackInfo& info) {
    TWAeternityAddressDelete(info[0].As<External<TWAeternityAddress>>().Data());
}

Boolean Js_wallet_core_napi_AeternityAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAeternityAddress *lhsInstance = info[0].As<External<TWAeternityAddress>>().Data();
    struct TWAeternityAddress *rhsInstance = info[1].As<External<TWAeternityAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWAeternityAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_AeternityAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWAeternityAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_AeternityAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAeternityAddress *instance = info[0].As<External<TWAeternityAddress>>().Data();

    TWString* twResult = TWAeternityAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

