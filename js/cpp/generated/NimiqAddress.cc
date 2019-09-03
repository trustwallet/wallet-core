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

#include <TrustWalletCore/TWNimiqAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "NimiqAddress.h"

using namespace Napi;

External<TWNimiqAddress> Js_wallet_core_napi_NimiqAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWNimiqAddress *instance = TWNimiqAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNimiqAddress>::New(env, instance, [](Env _env, void* data) {
        TWNimiqAddressDelete(reinterpret_cast<TWNimiqAddress *>(data));
    });
}

External<TWNimiqAddress> Js_wallet_core_napi_NimiqAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWNimiqAddress *instance = TWNimiqAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWNimiqAddress>::New(env, instance, [](Env _env, void* data) {
        TWNimiqAddressDelete(reinterpret_cast<TWNimiqAddress *>(data));
    });
}

void Js_wallet_core_napi_NimiqAddress_delete(const CallbackInfo& info) {
    TWNimiqAddressDelete(info[0].As<External<TWNimiqAddress>>().Data());
}

Boolean Js_wallet_core_napi_NimiqAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNimiqAddress *lhsInstance = info[0].As<External<TWNimiqAddress>>().Data();
    struct TWNimiqAddress *rhsInstance = info[1].As<External<TWNimiqAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWNimiqAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_NimiqAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWNimiqAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_NimiqAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNimiqAddress *instance = info[0].As<External<TWNimiqAddress>>().Data();

    TWString* twResult = TWNimiqAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NimiqAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNimiqAddress *instance = info[0].As<External<TWNimiqAddress>>().Data();

    TWData* twResult = TWNimiqAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

