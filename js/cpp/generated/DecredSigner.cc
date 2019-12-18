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

#include <TrustWalletCore/TWDecredSigner.h>

#include "../TWNapi.h"
#include "DecredSigner.h"

using namespace Napi;

External<TWDecredSigner> Js_wallet_core_napi_DecredSigner_create(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    struct TWDecredSigner *instance = TWDecredSignerCreate(inputData);
    return External<TWDecredSigner>::New(env, instance, [](Env _env, void* data) {
        TWDecredSignerDelete(reinterpret_cast<TWDecredSigner *>(data));
    });
}

External<TWDecredSigner> Js_wallet_core_napi_DecredSigner_createWithPlan(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    Uint8Array planByteArray = info[1].As<Uint8Array>();
    TWData *planData = TWDataCreateWithUint8Array(planByteArray);
    struct TWDecredSigner *instance = TWDecredSignerCreateWithPlan(inputData, planData);
    return External<TWDecredSigner>::New(env, instance, [](Env _env, void* data) {
        TWDecredSignerDelete(reinterpret_cast<TWDecredSigner *>(data));
    });
}

void Js_wallet_core_napi_DecredSigner_delete(const CallbackInfo& info) {
    TWDecredSignerDelete(info[0].As<External<TWDecredSigner>>().Data());
}

Uint8Array Js_wallet_core_napi_DecredSigner_plan(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWDecredSigner *instance = info[0].As<External<TWDecredSigner>>().Data();

    TWData* twResult = TWDecredSignerPlan(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_DecredSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWDecredSigner *instance = info[0].As<External<TWDecredSigner>>().Data();

    TWData* twResult = TWDecredSignerSign(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

