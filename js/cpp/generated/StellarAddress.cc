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
#include <TrustWalletCore/TWStellarAddress.h>

#include "../TWNapi.h"
#include "StellarAddress.h"

using namespace Napi;

External<TWStellarAddress> Js_wallet_core_napi_StellarAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWStellarAddress *instance = TWStellarAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWStellarAddress>::New(env, instance, [](Env _env, void* data) {
        TWStellarAddressDelete(reinterpret_cast<TWStellarAddress *>(data));
    });
}

External<TWStellarAddress> Js_wallet_core_napi_StellarAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWStellarAddress *instance = TWStellarAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWStellarAddress>::New(env, instance, [](Env _env, void* data) {
        TWStellarAddressDelete(reinterpret_cast<TWStellarAddress *>(data));
    });
}

void Js_wallet_core_napi_StellarAddress_delete(const CallbackInfo& info) {
    TWStellarAddressDelete(info[0].As<External<TWStellarAddress>>().Data());
}

Boolean Js_wallet_core_napi_StellarAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStellarAddress *lhsInstance = info[0].As<External<TWStellarAddress>>().Data();
    struct TWStellarAddress *rhsInstance = info[1].As<External<TWStellarAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWStellarAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_StellarAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWStellarAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_StellarAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStellarAddress *instance = info[0].As<External<TWStellarAddress>>().Data();

    TWString* twResult = TWStellarAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_StellarAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStellarAddress *instance = info[0].As<External<TWStellarAddress>>().Data();

    TWData* twResult = TWStellarAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

