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

#include <TrustWalletCore/TWNEOAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "NEOAddress.h"

using namespace Napi;

External<TWNEOAddress> Js_wallet_core_napi_NEOAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWNEOAddress *instance = TWNEOAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNEOAddress>::New(env, instance, [](Env _env, void* data) {
        TWNEOAddressDelete(reinterpret_cast<TWNEOAddress *>(data));
    });
}

External<TWNEOAddress> Js_wallet_core_napi_NEOAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWNEOAddress *instance = TWNEOAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWNEOAddress>::New(env, instance, [](Env _env, void* data) {
        TWNEOAddressDelete(reinterpret_cast<TWNEOAddress *>(data));
    });
}

void Js_wallet_core_napi_NEOAddress_delete(const CallbackInfo& info) {
    TWNEOAddressDelete(info[0].As<External<TWNEOAddress>>().Data());
}

Boolean Js_wallet_core_napi_NEOAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNEOAddress *lhsInstance = info[0].As<External<TWNEOAddress>>().Data();
    struct TWNEOAddress *rhsInstance = info[1].As<External<TWNEOAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWNEOAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_NEOAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWNEOAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_NEOAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNEOAddress *instance = info[0].As<External<TWNEOAddress>>().Data();

    TWString* twResult = TWNEOAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NEOAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNEOAddress *instance = info[0].As<External<TWNEOAddress>>().Data();

    TWData* twResult = TWNEOAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

