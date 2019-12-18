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

#include <TrustWalletCore/TWEthereumAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "EthereumAddress.h"

using namespace Napi;

External<TWEthereumAddress> Js_wallet_core_napi_EthereumAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWEthereumAddress *instance = TWEthereumAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWEthereumAddress>::New(env, instance, [](Env _env, void* data) {
        TWEthereumAddressDelete(reinterpret_cast<TWEthereumAddress *>(data));
    });
}

External<TWEthereumAddress> Js_wallet_core_napi_EthereumAddress_createWithKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *keyHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWEthereumAddress *instance = TWEthereumAddressCreateWithKeyHash(keyHashData);
    TWDataDelete(keyHashData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWEthereumAddress>::New(env, instance, [](Env _env, void* data) {
        TWEthereumAddressDelete(reinterpret_cast<TWEthereumAddress *>(data));
    });
}

External<TWEthereumAddress> Js_wallet_core_napi_EthereumAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWEthereumAddress *instance = TWEthereumAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWEthereumAddress>::New(env, instance, [](Env _env, void* data) {
        TWEthereumAddressDelete(reinterpret_cast<TWEthereumAddress *>(data));
    });
}

void Js_wallet_core_napi_EthereumAddress_delete(const CallbackInfo& info) {
    TWEthereumAddressDelete(info[0].As<External<TWEthereumAddress>>().Data());
}

Boolean Js_wallet_core_napi_EthereumAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWEthereumAddress *lhsInstance = info[0].As<External<TWEthereumAddress>>().Data();
    struct TWEthereumAddress *rhsInstance = info[1].As<External<TWEthereumAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWEthereumAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_EthereumAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWEthereumAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_EthereumAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWEthereumAddress *instance = info[0].As<External<TWEthereumAddress>>().Data();

    TWString* twResult = TWEthereumAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_EthereumAddress_keyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWEthereumAddress *instance = info[0].As<External<TWEthereumAddress>>().Data();

    TWData* twResult = TWEthereumAddressKeyHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

