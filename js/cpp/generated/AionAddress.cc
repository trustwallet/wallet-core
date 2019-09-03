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

#include <TrustWalletCore/TWAionAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "AionAddress.h"

using namespace Napi;

External<TWAionAddress> Js_wallet_core_napi_AionAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWAionAddress *instance = TWAionAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWAionAddress>::New(env, instance, [](Env _env, void* data) {
        TWAionAddressDelete(reinterpret_cast<TWAionAddress *>(data));
    });
}

External<TWAionAddress> Js_wallet_core_napi_AionAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWAionAddress *instance = TWAionAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWAionAddress>::New(env, instance, [](Env _env, void* data) {
        TWAionAddressDelete(reinterpret_cast<TWAionAddress *>(data));
    });
}

void Js_wallet_core_napi_AionAddress_delete(const CallbackInfo& info) {
    TWAionAddressDelete(info[0].As<External<TWAionAddress>>().Data());
}

Boolean Js_wallet_core_napi_AionAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAionAddress *lhsInstance = info[0].As<External<TWAionAddress>>().Data();
    struct TWAionAddress *rhsInstance = info[1].As<External<TWAionAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWAionAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_AionAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWAionAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_AionAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAionAddress *instance = info[0].As<External<TWAionAddress>>().Data();

    TWString* twResult = TWAionAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

