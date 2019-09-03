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

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "BitcoinAddress.h"

using namespace Napi;

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWBitcoinAddress *instance = TWBitcoinAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWBitcoinAddress>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinAddressDelete(reinterpret_cast<TWBitcoinAddress *>(data));
    });
}

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinAddress *instance = TWBitcoinAddressCreateWithData(dataData);
    TWDataDelete(dataData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWBitcoinAddress>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinAddressDelete(reinterpret_cast<TWBitcoinAddress *>(data));
    });
}

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    uint8_t prefix = info[1].As<Number>().Int64Value();
    struct TWBitcoinAddress *instance = TWBitcoinAddressCreateWithPublicKey(publicKeyInstance, prefix);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWBitcoinAddress>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinAddressDelete(reinterpret_cast<TWBitcoinAddress *>(data));
    });
}

void Js_wallet_core_napi_BitcoinAddress_delete(const CallbackInfo& info) {
    TWBitcoinAddressDelete(info[0].As<External<TWBitcoinAddress>>().Data());
}

Boolean Js_wallet_core_napi_BitcoinAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinAddress *lhsInstance = info[0].As<External<TWBitcoinAddress>>().Data();
    struct TWBitcoinAddress *rhsInstance = info[1].As<External<TWBitcoinAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWBitcoinAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinAddress_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    Boolean resultValue = Boolean::New(env, TWBitcoinAddressIsValid(dataData));
    TWDataDelete(dataData);
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWBitcoinAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_BitcoinAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinAddress *instance = info[0].As<External<TWBitcoinAddress>>().Data();

    TWString* twResult = TWBitcoinAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_BitcoinAddress_prefix(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinAddress *instance = info[0].As<External<TWBitcoinAddress>>().Data();

    Number resultValue = Number::New(env, TWBitcoinAddressPrefix(instance));
    return resultValue;
}

Uint8Array Js_wallet_core_napi_BitcoinAddress_keyhash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinAddress *instance = info[0].As<External<TWBitcoinAddress>>().Data();

    TWData* twResult = TWBitcoinAddressKeyhash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

