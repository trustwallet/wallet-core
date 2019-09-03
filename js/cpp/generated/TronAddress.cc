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

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWTronAddress.h>

#include "../TWNapi.h"
#include "TronAddress.h"

using namespace Napi;

External<TWTronAddress> Js_wallet_core_napi_TronAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWTronAddress *instance = TWTronAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWTronAddress>::New(env, instance, [](Env _env, void* data) {
        TWTronAddressDelete(reinterpret_cast<TWTronAddress *>(data));
    });
}

External<TWTronAddress> Js_wallet_core_napi_TronAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWTronAddress *instance = TWTronAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWTronAddress>::New(env, instance, [](Env _env, void* data) {
        TWTronAddressDelete(reinterpret_cast<TWTronAddress *>(data));
    });
}

void Js_wallet_core_napi_TronAddress_delete(const CallbackInfo& info) {
    TWTronAddressDelete(info[0].As<External<TWTronAddress>>().Data());
}

Boolean Js_wallet_core_napi_TronAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWTronAddress *lhsInstance = info[0].As<External<TWTronAddress>>().Data();
    struct TWTronAddress *rhsInstance = info[1].As<External<TWTronAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWTronAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_TronAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWTronAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_TronAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWTronAddress *instance = info[0].As<External<TWTronAddress>>().Data();

    TWString* twResult = TWTronAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

