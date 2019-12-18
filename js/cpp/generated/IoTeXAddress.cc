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

#include <TrustWalletCore/TWIoTeXAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "IoTeXAddress.h"

using namespace Napi;

External<TWIoTeXAddress> Js_wallet_core_napi_IoTeXAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWIoTeXAddress *instance = TWIoTeXAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWIoTeXAddress>::New(env, instance, [](Env _env, void* data) {
        TWIoTeXAddressDelete(reinterpret_cast<TWIoTeXAddress *>(data));
    });
}

External<TWIoTeXAddress> Js_wallet_core_napi_IoTeXAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWIoTeXAddress *instance = TWIoTeXAddressCreateWithKeyHash(keyHashData);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWIoTeXAddress>::New(env, instance, [](Env _env, void* data) {
        TWIoTeXAddressDelete(reinterpret_cast<TWIoTeXAddress *>(data));
    });
}

External<TWIoTeXAddress> Js_wallet_core_napi_IoTeXAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWIoTeXAddress *instance = TWIoTeXAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWIoTeXAddress>::New(env, instance, [](Env _env, void* data) {
        TWIoTeXAddressDelete(reinterpret_cast<TWIoTeXAddress *>(data));
    });
}

void Js_wallet_core_napi_IoTeXAddress_delete(const CallbackInfo& info) {
    TWIoTeXAddressDelete(info[0].As<External<TWIoTeXAddress>>().Data());
}

Boolean Js_wallet_core_napi_IoTeXAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIoTeXAddress *lhsInstance = info[0].As<External<TWIoTeXAddress>>().Data();
    struct TWIoTeXAddress *rhsInstance = info[1].As<External<TWIoTeXAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWIoTeXAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_IoTeXAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWIoTeXAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_IoTeXAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIoTeXAddress *instance = info[0].As<External<TWIoTeXAddress>>().Data();

    TWString* twResult = TWIoTeXAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_IoTeXAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIoTeXAddress *instance = info[0].As<External<TWIoTeXAddress>>().Data();

    TWData* twResult = TWIoTeXAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

