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

#include <TrustWalletCore/TWBitcoinCashAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "BitcoinCashAddress.h"

using namespace Napi;

Value Js_wallet_core_napi_BitcoinCashAddress_initWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWBitcoinCashAddress* instance = new TWBitcoinCashAddress();
    bool result = TWBitcoinCashAddressInitWithString(instance, stringString);
    TWStringDelete(stringString);

    if (result) {
        return External<TWBitcoinCashAddress>::New(env, instance, [](Env _env, void* data) {
            delete reinterpret_cast<TWBitcoinCashAddress *>(data);
        });
    } else {
        return env.Null();
    }
}

Value Js_wallet_core_napi_BitcoinCashAddress_initWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinCashAddress* instance = new TWBitcoinCashAddress();
    bool result = TWBitcoinCashAddressInitWithData(instance, dataData);
    TWDataDelete(dataData);

    if (result) {
        return External<TWBitcoinCashAddress>::New(env, instance, [](Env _env, void* data) {
            delete reinterpret_cast<TWBitcoinCashAddress *>(data);
        });
    } else {
        return env.Null();
    }
}

Value Js_wallet_core_napi_BitcoinCashAddress_initWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWBitcoinCashAddress* instance = new TWBitcoinCashAddress();
    TWBitcoinCashAddressInitWithPublicKey(instance, publicKeyInstance);

    return External<TWBitcoinCashAddress>::New(env, instance, [](Env _env, void* data) {
        delete reinterpret_cast<TWBitcoinCashAddress *>(data);
    });
}

Boolean Js_wallet_core_napi_BitcoinCashAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinCashAddress *lhsInstance = info[0].As<External<TWBitcoinCashAddress>>().Data();
    struct TWBitcoinCashAddress *rhsInstance = info[1].As<External<TWBitcoinCashAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWBitcoinCashAddressEqual(*lhsInstance, *rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinCashAddress_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    Boolean resultValue = Boolean::New(env, TWBitcoinCashAddressIsValid(dataData));
    TWDataDelete(dataData);
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinCashAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWBitcoinCashAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_BitcoinCashAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinCashAddress *instance = info[0].As<External<TWBitcoinCashAddress>>().Data();

    TWString* twResult = TWBitcoinCashAddressDescription(*instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Value Js_wallet_core_napi_BitcoinCashAddress_legacyAddress(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinCashAddress *instance = info[0].As<External<TWBitcoinCashAddress>>().Data();

    struct TWBitcoinAddress *result = TWBitcoinCashAddressLegacyAddress(*instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinAddress>::New(env, result, [](Env _env, void* data) {
        TWBitcoinAddressDelete(reinterpret_cast<TWBitcoinAddress *>(data));
    });
}

