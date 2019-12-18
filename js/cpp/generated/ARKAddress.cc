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

#include <TrustWalletCore/TWARKAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "ARKAddress.h"

using namespace Napi;

External<TWARKAddress> Js_wallet_core_napi_ARKAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWARKAddress *instance = TWARKAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWARKAddress>::New(env, instance, [](Env _env, void* data) {
        TWARKAddressDelete(reinterpret_cast<TWARKAddress *>(data));
    });
}

External<TWARKAddress> Js_wallet_core_napi_ARKAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWARKAddress *instance = TWARKAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWARKAddress>::New(env, instance, [](Env _env, void* data) {
        TWARKAddressDelete(reinterpret_cast<TWARKAddress *>(data));
    });
}

void Js_wallet_core_napi_ARKAddress_delete(const CallbackInfo& info) {
    TWARKAddressDelete(info[0].As<External<TWARKAddress>>().Data());
}

Boolean Js_wallet_core_napi_ARKAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWARKAddress *lhsInstance = info[0].As<External<TWARKAddress>>().Data();
    struct TWARKAddress *rhsInstance = info[1].As<External<TWARKAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWARKAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_ARKAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWARKAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_ARKAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWARKAddress *instance = info[0].As<External<TWARKAddress>>().Data();

    TWString* twResult = TWARKAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

