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
#include <TrustWalletCore/TWSegwitAddress.h>

#include "../TWNapi.h"
#include "SegwitAddress.h"

using namespace Napi;

External<TWSegwitAddress> Js_wallet_core_napi_SegwitAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWSegwitAddress *instance = TWSegwitAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWSegwitAddress>::New(env, instance, [](Env _env, void* data) {
        TWSegwitAddressDelete(reinterpret_cast<TWSegwitAddress *>(data));
    });
}

External<TWSegwitAddress> Js_wallet_core_napi_SegwitAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWHRP hrpValue = (TWHRP)((int) info[0].As<Number>());
    struct TWPublicKey *publicKeyInstance = info[1].As<External<TWPublicKey>>().Data();
    struct TWSegwitAddress *instance = TWSegwitAddressCreateWithPublicKey(hrpValue, publicKeyInstance);
    return External<TWSegwitAddress>::New(env, instance, [](Env _env, void* data) {
        TWSegwitAddressDelete(reinterpret_cast<TWSegwitAddress *>(data));
    });
}

void Js_wallet_core_napi_SegwitAddress_delete(const CallbackInfo& info) {
    TWSegwitAddressDelete(info[0].As<External<TWSegwitAddress>>().Data());
}

Boolean Js_wallet_core_napi_SegwitAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSegwitAddress *lhsInstance = info[0].As<External<TWSegwitAddress>>().Data();
    struct TWSegwitAddress *rhsInstance = info[1].As<External<TWSegwitAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWSegwitAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_SegwitAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWSegwitAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_SegwitAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSegwitAddress *instance = info[0].As<External<TWSegwitAddress>>().Data();

    TWString* twResult = TWSegwitAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_SegwitAddress_hrp(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSegwitAddress *instance = info[0].As<External<TWSegwitAddress>>().Data();

    enum TWHRP result = TWSegwitAddressHRP(instance);
    return Number::New(env, (int) result);;
}

Uint8Array Js_wallet_core_napi_SegwitAddress_witnessProgram(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSegwitAddress *instance = info[0].As<External<TWSegwitAddress>>().Data();

    TWData* twResult = TWSegwitAddressWitnessProgram(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

