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
#include <TrustWalletCore/TWWavesAddress.h>

#include "../TWNapi.h"
#include "WavesAddress.h"

using namespace Napi;

External<TWWavesAddress> Js_wallet_core_napi_WavesAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWWavesAddress *instance = TWWavesAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWWavesAddress>::New(env, instance, [](Env _env, void* data) {
        TWWavesAddressDelete(reinterpret_cast<TWWavesAddress *>(data));
    });
}

External<TWWavesAddress> Js_wallet_core_napi_WavesAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWWavesAddress *instance = TWWavesAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWWavesAddress>::New(env, instance, [](Env _env, void* data) {
        TWWavesAddressDelete(reinterpret_cast<TWWavesAddress *>(data));
    });
}

void Js_wallet_core_napi_WavesAddress_delete(const CallbackInfo& info) {
    TWWavesAddressDelete(info[0].As<External<TWWavesAddress>>().Data());
}

Boolean Js_wallet_core_napi_WavesAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWavesAddress *lhsInstance = info[0].As<External<TWWavesAddress>>().Data();
    struct TWWavesAddress *rhsInstance = info[1].As<External<TWWavesAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWWavesAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_WavesAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWWavesAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_WavesAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWavesAddress *instance = info[0].As<External<TWWavesAddress>>().Data();

    TWString* twResult = TWWavesAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_WavesAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWWavesAddress *instance = info[0].As<External<TWWavesAddress>>().Data();

    TWData* twResult = TWWavesAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

