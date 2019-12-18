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
#include <TrustWalletCore/TWSemuxAddress.h>

#include "../TWNapi.h"
#include "SemuxAddress.h"

using namespace Napi;

External<TWSemuxAddress> Js_wallet_core_napi_SemuxAddress_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWSemuxAddress *instance = TWSemuxAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWSemuxAddress>::New(env, instance, [](Env _env, void* data) {
        TWSemuxAddressDelete(reinterpret_cast<TWSemuxAddress *>(data));
    });
}

External<TWSemuxAddress> Js_wallet_core_napi_SemuxAddress_createWithPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPublicKey *publicKeyInstance = info[0].As<External<TWPublicKey>>().Data();
    struct TWSemuxAddress *instance = TWSemuxAddressCreateWithPublicKey(publicKeyInstance);
    return External<TWSemuxAddress>::New(env, instance, [](Env _env, void* data) {
        TWSemuxAddressDelete(reinterpret_cast<TWSemuxAddress *>(data));
    });
}

void Js_wallet_core_napi_SemuxAddress_delete(const CallbackInfo& info) {
    TWSemuxAddressDelete(info[0].As<External<TWSemuxAddress>>().Data());
}

Boolean Js_wallet_core_napi_SemuxAddress_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSemuxAddress *lhsInstance = info[0].As<External<TWSemuxAddress>>().Data();
    struct TWSemuxAddress *rhsInstance = info[1].As<External<TWSemuxAddress>>().Data();
    Boolean resultValue = Boolean::New(env, TWSemuxAddressEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_SemuxAddress_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWSemuxAddressIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_SemuxAddress_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWSemuxAddress *instance = info[0].As<External<TWSemuxAddress>>().Data();

    TWString* twResult = TWSemuxAddressDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

