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

#include <TrustWalletCore/TWNebulasAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "NebulasAddress.h"

using namespace Napi;

External<TWNebulasAddress> Js_wallet_core_napi_NebulasAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWNebulasAddress *instance = TWNebulasAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWNebulasAddress>::New(env, instance, [](Env _env, void* data) {
        TWNebulasAddressDelete(reinterpret_cast<TWNebulasAddress *>(data));
    });
}

External<TWNebulasAddress> Js_wallet_core_napi_NebulasAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWNebulasAddress *instance = TWNebulasAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWNebulasAddress>::New(env, instance, [](Env _env, void* data) {
        TWNebulasAddressDelete(reinterpret_cast<TWNebulasAddress *>(data));
    });
}

void Js_wallet_core_napi_NebulasAddress_delete(const CallbackInfo& info) {
    TWNebulasAddressDelete(info[0].As<External<TWNebulasAddress>>().Data());
}

Boolean Js_wallet_core_napi_NebulasAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNebulasAddress *lhsInstance = info[0].As<External<TWNebulasAddress>>().Data();
    struct TWNebulasAddress *rhsInstance = info[1].As<External<TWNebulasAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWNebulasAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_NebulasAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWNebulasAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_NebulasAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNebulasAddress *instance = info[0].As<External<TWNebulasAddress>>().Data();

    TWString* twResult = TWNebulasAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NebulasAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWNebulasAddress *instance = info[0].As<External<TWNebulasAddress>>().Data();

    TWData* twResult = TWNebulasAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

