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
#include <TrustWalletCore/TWTezosAddress.h>

#include "../TWNapi.h"
#include "TezosAddress.h"

using namespace Napi;

External<TWTezosAddress> Js_wallet_core_napi_TezosAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWTezosAddress *instance = TWTezosAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWTezosAddress>::New(env, instance, [](Env _env, void* data) {
        TWTezosAddressDelete(reinterpret_cast<TWTezosAddress *>(data));
    });
}

External<TWTezosAddress> Js_wallet_core_napi_TezosAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWTezosAddress *instance = TWTezosAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWTezosAddress>::New(env, instance, [](Env _env, void* data) {
        TWTezosAddressDelete(reinterpret_cast<TWTezosAddress *>(data));
    });
}

void Js_wallet_core_napi_TezosAddress_delete(const CallbackInfo& info) {
    TWTezosAddressDelete(info[0].As<External<TWTezosAddress>>().Data());
}

Boolean Js_wallet_core_napi_TezosAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWTezosAddress *lhsInstance = info[0].As<External<TWTezosAddress>>().Data();
    struct TWTezosAddress *rhsInstance = info[1].As<External<TWTezosAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWTezosAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_TezosAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWTezosAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_TezosAddress_deriveOriginatedAddress(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *operationHashString = TWStringCreateWithJsString(info[0].As<String>());
    int operationIndex = info[1].As<Number>().Int64Value();
    TWString* twResult = TWTezosAddressDeriveOriginatedAddress(operationHashString, operationIndex);
    String result = TWStringJsString(&env, twResult);
    TWStringDelete(operationHashString);
    return result;
}

String Js_wallet_core_napi_TezosAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWTezosAddress *instance = info[0].As<External<TWTezosAddress>>().Data();

    TWString* twResult = TWTezosAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_TezosAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWTezosAddress *instance = info[0].As<External<TWTezosAddress>>().Data();

    TWData* twResult = TWTezosAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

