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

#include <TrustWalletCore/TWCosmosAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "CosmosAddress.h"

using namespace Napi;

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWCosmosAddress *instance = TWCosmosAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWCosmosAddress>::New(env, instance, [](Env _env, void* data) {
        TWCosmosAddressDelete(reinterpret_cast<TWCosmosAddress *>(data));
    });
}

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWHRP hrpValue = (TWHRP)((int) info[0].As<Number>());
    TWData *keyHashData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    struct TWCosmosAddress *instance = TWCosmosAddressCreateWithKeyHash(hrpValue, keyHashData);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWCosmosAddress>::New(env, instance, [](Env _env, void* data) {
        TWCosmosAddressDelete(reinterpret_cast<TWCosmosAddress *>(data));
    });
}

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWHRP hrpValue = (TWHRP)((int) info[0].As<Number>());
    struct TWPublicKey *publicKeyInstance = info[1].As<External<TWPublicKey>>().Data();
    struct TWCosmosAddress *instance = TWCosmosAddressCreateWithPublicKey(hrpValue, publicKeyInstance);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWCosmosAddress>::New(env, instance, [](Env _env, void* data) {
        TWCosmosAddressDelete(reinterpret_cast<TWCosmosAddress *>(data));
    });
}

void Js_wallet_core_napi_CosmosAddress_delete(const CallbackInfo& info) {
    TWCosmosAddressDelete(info[0].As<External<TWCosmosAddress>>().Data());
}

Boolean Js_wallet_core_napi_CosmosAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWCosmosAddress *lhsInstance = info[0].As<External<TWCosmosAddress>>().Data();
    struct TWCosmosAddress *rhsInstance = info[1].As<External<TWCosmosAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWCosmosAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_CosmosAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWCosmosAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_CosmosAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWCosmosAddress *instance = info[0].As<External<TWCosmosAddress>>().Data();

    TWString* twResult = TWCosmosAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_CosmosAddress_hrp(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWCosmosAddress *instance = info[0].As<External<TWCosmosAddress>>().Data();

    enum TWHRP result = TWCosmosAddressHRP(instance);
    return Number::New(env, (int) result);;
}

Uint8Array Js_wallet_core_napi_CosmosAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWCosmosAddress *instance = info[0].As<External<TWCosmosAddress>>().Data();

    TWData* twResult = TWCosmosAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

