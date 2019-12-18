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

#include <TrustWalletCore/TWNanoAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "NanoAddress.h"

using namespace Napi;

External<TWNanoAddress> Js_wallet_core_napi_NanoAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWNanoAddress *instance = TWNanoAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNanoAddress>::New(env, instance, [](Env _env, void* data) {
        TWNanoAddressDelete(reinterpret_cast<TWNanoAddress *>(data));
    });
}

External<TWNanoAddress> Js_wallet_core_napi_NanoAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWNanoAddress *instance = TWNanoAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWNanoAddress>::New(env, instance, [](Env _env, void* data) {
        TWNanoAddressDelete(reinterpret_cast<TWNanoAddress *>(data));
    });
}

void Js_wallet_core_napi_NanoAddress_delete(const CallbackInfo& info) {
    TWNanoAddressDelete(info[0].As<External<TWNanoAddress>>().Data());
}

Boolean Js_wallet_core_napi_NanoAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNanoAddress *lhsInstance = info[0].As<External<TWNanoAddress>>().Data();
    struct TWNanoAddress *rhsInstance = info[1].As<External<TWNanoAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWNanoAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_NanoAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWNanoAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_NanoAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNanoAddress *instance = info[0].As<External<TWNanoAddress>>().Data();

    TWString* twResult = TWNanoAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NanoAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNanoAddress *instance = info[0].As<External<TWNanoAddress>>().Data();

    TWData* twResult = TWNanoAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

