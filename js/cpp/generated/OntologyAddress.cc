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

#include <TrustWalletCore/TWOntologyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "OntologyAddress.h"

using namespace Napi;

External<TWOntologyAddress> Js_wallet_core_napi_OntologyAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWOntologyAddress *instance = TWOntologyAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWOntologyAddress>::New(env, instance, [](Env _env, void* data) {
        TWOntologyAddressDelete(reinterpret_cast<TWOntologyAddress *>(data));
    });
}

External<TWOntologyAddress> Js_wallet_core_napi_OntologyAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWOntologyAddress *instance = TWOntologyAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWOntologyAddress>::New(env, instance, [](Env _env, void* data) {
        TWOntologyAddressDelete(reinterpret_cast<TWOntologyAddress *>(data));
    });
}

void Js_wallet_core_napi_OntologyAddress_delete(const CallbackInfo& info) {
    TWOntologyAddressDelete(info[0].As<External<TWOntologyAddress>>().Data());
}

Boolean Js_wallet_core_napi_OntologyAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWOntologyAddress *lhsInstance = info[0].As<External<TWOntologyAddress>>().Data();
    struct TWOntologyAddress *rhsInstance = info[1].As<External<TWOntologyAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWOntologyAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_OntologyAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWOntologyAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_OntologyAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWOntologyAddress *instance = info[0].As<External<TWOntologyAddress>>().Data();

    TWString* twResult = TWOntologyAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_OntologyAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWOntologyAddress *instance = info[0].As<External<TWOntologyAddress>>().Data();

    TWData* twResult = TWOntologyAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

