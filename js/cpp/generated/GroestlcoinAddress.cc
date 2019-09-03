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

#include <TrustWalletCore/TWGroestlcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "GroestlcoinAddress.h"

using namespace Napi;

External<TWGroestlcoinAddress> Js_wallet_core_napi_GroestlcoinAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWGroestlcoinAddress *instance = TWGroestlcoinAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWGroestlcoinAddress>::New(env, instance, [](Env _env, void* data) {
        TWGroestlcoinAddressDelete(reinterpret_cast<TWGroestlcoinAddress *>(data));
    });
}

External<TWGroestlcoinAddress> Js_wallet_core_napi_GroestlcoinAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    uint8_t prefix = info[1].As<Number>().Int64Value();
    struct TWGroestlcoinAddress *instance = TWGroestlcoinAddressCreateWithPublicKey(publicKeyInstance, prefix);
    return External<TWGroestlcoinAddress>::New(env, instance, [](Env _env, void* data) {
        TWGroestlcoinAddressDelete(reinterpret_cast<TWGroestlcoinAddress *>(data));
    });
}

void Js_wallet_core_napi_GroestlcoinAddress_delete(const CallbackInfo& info) {
    TWGroestlcoinAddressDelete(info[0].As<External<TWGroestlcoinAddress>>().Data());
}

Boolean Js_wallet_core_napi_GroestlcoinAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWGroestlcoinAddress *lhsInstance = info[0].As<External<TWGroestlcoinAddress>>().Data();
    struct TWGroestlcoinAddress *rhsInstance = info[1].As<External<TWGroestlcoinAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWGroestlcoinAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_GroestlcoinAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWGroestlcoinAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_GroestlcoinAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWGroestlcoinAddress *instance = info[0].As<External<TWGroestlcoinAddress>>().Data();

    TWString* twResult = TWGroestlcoinAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

