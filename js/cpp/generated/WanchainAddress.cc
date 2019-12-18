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
#include <TrustWalletCore/TWWanchainAddress.h>

#include "../TWNapi.h"
#include "WanchainAddress.h"

using namespace Napi;

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWWanchainAddress *instance = TWWanchainAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWWanchainAddress>::New(env, instance, [](Env _env, void* data) {
        TWWanchainAddressDelete(reinterpret_cast<TWWanchainAddress *>(data));
    });
}

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWWanchainAddress *instance = TWWanchainAddressCreateWithKeyHash(keyHashData);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWWanchainAddress>::New(env, instance, [](Env _env, void* data) {
        TWWanchainAddressDelete(reinterpret_cast<TWWanchainAddress *>(data));
    });
}

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWWanchainAddress *instance = TWWanchainAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWWanchainAddress>::New(env, instance, [](Env _env, void* data) {
        TWWanchainAddressDelete(reinterpret_cast<TWWanchainAddress *>(data));
    });
}

void Js_wallet_core_napi_WanchainAddress_delete(const CallbackInfo& info) {
    TWWanchainAddressDelete(info[0].As<External<TWWanchainAddress>>().Data());
}

Boolean Js_wallet_core_napi_WanchainAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWanchainAddress *lhsInstance = info[0].As<External<TWWanchainAddress>>().Data();
    struct TWWanchainAddress *rhsInstance = info[1].As<External<TWWanchainAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWWanchainAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_WanchainAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWWanchainAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_WanchainAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWanchainAddress *instance = info[0].As<External<TWWanchainAddress>>().Data();

    TWString* twResult = TWWanchainAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_WanchainAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWanchainAddress *instance = info[0].As<External<TWWanchainAddress>>().Data();

    TWData* twResult = TWWanchainAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

