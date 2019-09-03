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

#include <TrustWalletCore/TWZcashTransactionSigner.h>

#include "../TWNapi.h"
#include "ZcashTransactionSigner.h"

using namespace Napi;

External<TWZcashTransactionSigner> Js_wallet_core_napi_ZcashTransactionSigner_create(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    struct TWZcashTransactionSigner *instance = TWZcashTransactionSignerCreate(inputData);
    return External<TWZcashTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWZcashTransactionSignerDelete(reinterpret_cast<TWZcashTransactionSigner *>(data));
    });
}

External<TWZcashTransactionSigner> Js_wallet_core_napi_ZcashTransactionSigner_createWithPlan(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    Uint8Array planByteArray = info[1].As<Uint8Array>();
    TWData *planData = TWDataCreateWithUint8Array(planByteArray);
    struct TWZcashTransactionSigner *instance = TWZcashTransactionSignerCreateWithPlan(inputData, planData);
    return External<TWZcashTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWZcashTransactionSignerDelete(reinterpret_cast<TWZcashTransactionSigner *>(data));
    });
}

void Js_wallet_core_napi_ZcashTransactionSigner_delete(const CallbackInfo& info) {
    TWZcashTransactionSignerDelete(info[0].As<External<TWZcashTransactionSigner>>().Data());
}

Uint8Array Js_wallet_core_napi_ZcashTransactionSigner_plan(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZcashTransactionSigner *instance = info[0].As<External<TWZcashTransactionSigner>>().Data();

    TWData* twResult = TWZcashTransactionSignerPlan(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_ZcashTransactionSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWZcashTransactionSigner *instance = info[0].As<External<TWZcashTransactionSigner>>().Data();

    TWData* twResult = TWZcashTransactionSignerSign(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

