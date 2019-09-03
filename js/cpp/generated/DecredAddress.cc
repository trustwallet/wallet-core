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

#include <TrustWalletCore/TWDecredAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "DecredAddress.h"

using namespace Napi;

External<TWDecredAddress> Js_wallet_core_napi_DecredAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWDecredAddress *instance = TWDecredAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWDecredAddress>::New(env, instance, [](Env _env, void* data) {
        TWDecredAddressDelete(reinterpret_cast<TWDecredAddress *>(data));
    });
}

External<TWDecredAddress> Js_wallet_core_napi_DecredAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWDecredAddress *instance = TWDecredAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWDecredAddress>::New(env, instance, [](Env _env, void* data) {
        TWDecredAddressDelete(reinterpret_cast<TWDecredAddress *>(data));
    });
}

void Js_wallet_core_napi_DecredAddress_delete(const CallbackInfo& info) {
    TWDecredAddressDelete(info[0].As<External<TWDecredAddress>>().Data());
}

Boolean Js_wallet_core_napi_DecredAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWDecredAddress *lhsInstance = info[0].As<External<TWDecredAddress>>().Data();
    struct TWDecredAddress *rhsInstance = info[1].As<External<TWDecredAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWDecredAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_DecredAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWDecredAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_DecredAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWDecredAddress *instance = info[0].As<External<TWDecredAddress>>().Data();

    TWString* twResult = TWDecredAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

