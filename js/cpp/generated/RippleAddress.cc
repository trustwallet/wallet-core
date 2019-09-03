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
#include <TrustWalletCore/TWRippleAddress.h>

#include "../TWNapi.h"
#include "RippleAddress.h"

using namespace Napi;

External<TWRippleAddress> Js_wallet_core_napi_RippleAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWRippleAddress *instance = TWRippleAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWRippleAddress>::New(env, instance, [](Env _env, void* data) {
        TWRippleAddressDelete(reinterpret_cast<TWRippleAddress *>(data));
    });
}

External<TWRippleAddress> Js_wallet_core_napi_RippleAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWRippleAddress *instance = TWRippleAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWRippleAddress>::New(env, instance, [](Env _env, void* data) {
        TWRippleAddressDelete(reinterpret_cast<TWRippleAddress *>(data));
    });
}

void Js_wallet_core_napi_RippleAddress_delete(const CallbackInfo& info) {
    TWRippleAddressDelete(info[0].As<External<TWRippleAddress>>().Data());
}

Boolean Js_wallet_core_napi_RippleAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWRippleAddress *lhsInstance = info[0].As<External<TWRippleAddress>>().Data();
    struct TWRippleAddress *rhsInstance = info[1].As<External<TWRippleAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWRippleAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_RippleAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWRippleAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_RippleAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWRippleAddress *instance = info[0].As<External<TWRippleAddress>>().Data();

    TWString* twResult = TWRippleAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_RippleAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWRippleAddress *instance = info[0].As<External<TWRippleAddress>>().Data();

    TWData* twResult = TWRippleAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

