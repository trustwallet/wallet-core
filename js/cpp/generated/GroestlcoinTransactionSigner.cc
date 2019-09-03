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

#include <TrustWalletCore/TWGroestlcoinTransactionSigner.h>

#include "../TWNapi.h"
#include "GroestlcoinTransactionSigner.h"

using namespace Napi;

External<TWGroestlcoinTransactionSigner> Js_wallet_core_napi_GroestlcoinTransactionSigner_create(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    struct TWGroestlcoinTransactionSigner *instance = TWGroestlcoinTransactionSignerCreate(inputData);
    return External<TWGroestlcoinTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWGroestlcoinTransactionSignerDelete(reinterpret_cast<TWGroestlcoinTransactionSigner *>(data));
    });
}

External<TWGroestlcoinTransactionSigner> Js_wallet_core_napi_GroestlcoinTransactionSigner_createWithPlan(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    Uint8Array planByteArray = info[1].As<Uint8Array>();
    TWData *planData = TWDataCreateWithUint8Array(planByteArray);
    struct TWGroestlcoinTransactionSigner *instance = TWGroestlcoinTransactionSignerCreateWithPlan(inputData, planData);
    return External<TWGroestlcoinTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWGroestlcoinTransactionSignerDelete(reinterpret_cast<TWGroestlcoinTransactionSigner *>(data));
    });
}

void Js_wallet_core_napi_GroestlcoinTransactionSigner_delete(const CallbackInfo& info) {
    TWGroestlcoinTransactionSignerDelete(info[0].As<External<TWGroestlcoinTransactionSigner>>().Data());
}

Uint8Array Js_wallet_core_napi_GroestlcoinTransactionSigner_plan(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWGroestlcoinTransactionSigner *instance = info[0].As<External<TWGroestlcoinTransactionSigner>>().Data();

    TWData* twResult = TWGroestlcoinTransactionSignerPlan(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_GroestlcoinTransactionSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWGroestlcoinTransactionSigner *instance = info[0].As<External<TWGroestlcoinTransactionSigner>>().Data();

    TWData* twResult = TWGroestlcoinTransactionSignerSign(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

