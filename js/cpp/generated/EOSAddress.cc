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

#include <TrustWalletCore/TWEOSAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "EOSAddress.h"

using namespace Napi;

External<TWEOSAddress> Js_wallet_core_napi_EOSAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWEOSAddress *instance = TWEOSAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWEOSAddress>::New(env, instance, [](Env _env, void* data) {
        TWEOSAddressDelete(reinterpret_cast<TWEOSAddress *>(data));
    });
}

External<TWEOSAddress> Js_wallet_core_napi_EOSAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    TWEOSKeyType typeValue = (TWEOSKeyType)((int) info[1].As<Number>());
    struct TWEOSAddress *instance = TWEOSAddressCreateWithPublicKey(publicKeyInstance, typeValue);
    return External<TWEOSAddress>::New(env, instance, [](Env _env, void* data) {
        TWEOSAddressDelete(reinterpret_cast<TWEOSAddress *>(data));
    });
}

External<TWEOSAddress> Js_wallet_core_napi_EOSAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWEOSKeyType typeValue = (TWEOSKeyType)((int) info[1].As<Number>());
    struct TWEOSAddress *instance = TWEOSAddressCreateWithKeyHash(keyHashData, typeValue);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWEOSAddress>::New(env, instance, [](Env _env, void* data) {
        TWEOSAddressDelete(reinterpret_cast<TWEOSAddress *>(data));
    });
}

void Js_wallet_core_napi_EOSAddress_delete(const CallbackInfo& info) {
    TWEOSAddressDelete(info[0].As<External<TWEOSAddress>>().Data());
}

Boolean Js_wallet_core_napi_EOSAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWEOSAddress *lhsInstance = info[0].As<External<TWEOSAddress>>().Data();
    struct TWEOSAddress *rhsInstance = info[1].As<External<TWEOSAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWEOSAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_EOSAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWEOSAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_EOSAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWEOSAddress *instance = info[0].As<External<TWEOSAddress>>().Data();

    TWString* twResult = TWEOSAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

