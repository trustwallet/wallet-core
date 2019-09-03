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
#include <TrustWalletCore/TWSteemAddress.h>

#include "../TWNapi.h"
#include "SteemAddress.h"

using namespace Napi;

External<TWSteemAddress> Js_wallet_core_napi_SteemAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWSteemAddress *instance = TWSteemAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWSteemAddress>::New(env, instance, [](Env _env, void* data) {
        TWSteemAddressDelete(reinterpret_cast<TWSteemAddress *>(data));
    });
}

External<TWSteemAddress> Js_wallet_core_napi_SteemAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    TWBravoAddressType typeValue = (TWBravoAddressType)((int) info[1].As<Number>());
    struct TWSteemAddress *instance = TWSteemAddressCreateWithPublicKey(publicKeyInstance, typeValue);
    return External<TWSteemAddress>::New(env, instance, [](Env _env, void* data) {
        TWSteemAddressDelete(reinterpret_cast<TWSteemAddress *>(data));
    });
}

External<TWSteemAddress> Js_wallet_core_napi_SteemAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWBravoAddressType typeValue = (TWBravoAddressType)((int) info[1].As<Number>());
    struct TWSteemAddress *instance = TWSteemAddressCreateWithKeyHash(keyHashData, typeValue);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWSteemAddress>::New(env, instance, [](Env _env, void* data) {
        TWSteemAddressDelete(reinterpret_cast<TWSteemAddress *>(data));
    });
}

void Js_wallet_core_napi_SteemAddress_delete(const CallbackInfo& info) {
    TWSteemAddressDelete(info[0].As<External<TWSteemAddress>>().Data());
}

Boolean Js_wallet_core_napi_SteemAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSteemAddress *lhsInstance = info[0].As<External<TWSteemAddress>>().Data();
    struct TWSteemAddress *rhsInstance = info[1].As<External<TWSteemAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWSteemAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_SteemAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWSteemAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_SteemAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSteemAddress *instance = info[0].As<External<TWSteemAddress>>().Data();

    TWString* twResult = TWSteemAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

