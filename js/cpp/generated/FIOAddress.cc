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

#include <TrustWalletCore/TWFIOAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "FIOAddress.h"

using namespace Napi;

External<TWFIOAddress> Js_wallet_core_napi_FIOAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWFIOAddress *instance = TWFIOAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWFIOAddress>::New(env, instance, [](Env _env, void* data) {
        TWFIOAddressDelete(reinterpret_cast<TWFIOAddress *>(data));
    });
}

External<TWFIOAddress> Js_wallet_core_napi_FIOAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWFIOAddress *instance = TWFIOAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWFIOAddress>::New(env, instance, [](Env _env, void* data) {
        TWFIOAddressDelete(reinterpret_cast<TWFIOAddress *>(data));
    });
}

void Js_wallet_core_napi_FIOAddress_delete(const CallbackInfo& info) {
    TWFIOAddressDelete(info[0].As<External<TWFIOAddress>>().Data());
}

Boolean Js_wallet_core_napi_FIOAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWFIOAddress *lhsInstance = info[0].As<External<TWFIOAddress>>().Data();
    struct TWFIOAddress *rhsInstance = info[1].As<External<TWFIOAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWFIOAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_FIOAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWFIOAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_FIOAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWFIOAddress *instance = info[0].As<External<TWFIOAddress>>().Data();

    TWString* twResult = TWFIOAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

