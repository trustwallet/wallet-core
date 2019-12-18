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

#include <TrustWalletCore/TWBravoAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "BravoAddress.h"

using namespace Napi;

External<TWBravoAddress> Js_wallet_core_napi_BravoAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWBravoAddress *instance = TWBravoAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWBravoAddress>::New(env, instance, [](Env _env, void* data) {
        TWBravoAddressDelete(reinterpret_cast<TWBravoAddress *>(data));
    });
}

External<TWBravoAddress> Js_wallet_core_napi_BravoAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    TWBravoAddressType typeValue = (TWBravoAddressType)((int) info[1].As<Number>());
    struct TWBravoAddress *instance = TWBravoAddressCreateWithPublicKey(publicKeyInstance, typeValue);
    return External<TWBravoAddress>::New(env, instance, [](Env _env, void* data) {
        TWBravoAddressDelete(reinterpret_cast<TWBravoAddress *>(data));
    });
}

External<TWBravoAddress> Js_wallet_core_napi_BravoAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWBravoAddressType typeValue = (TWBravoAddressType)((int) info[1].As<Number>());
    struct TWBravoAddress *instance = TWBravoAddressCreateWithKeyHash(keyHashData, typeValue);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWBravoAddress>::New(env, instance, [](Env _env, void* data) {
        TWBravoAddressDelete(reinterpret_cast<TWBravoAddress *>(data));
    });
}

void Js_wallet_core_napi_BravoAddress_delete(const CallbackInfo& info) {
    TWBravoAddressDelete(info[0].As<External<TWBravoAddress>>().Data());
}

Boolean Js_wallet_core_napi_BravoAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBravoAddress *lhsInstance = info[0].As<External<TWBravoAddress>>().Data();
    struct TWBravoAddress *rhsInstance = info[1].As<External<TWBravoAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWBravoAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BravoAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWBravoAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_BravoAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBravoAddress *instance = info[0].As<External<TWBravoAddress>>().Data();

    TWString* twResult = TWBravoAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

