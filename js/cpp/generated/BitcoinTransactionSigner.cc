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

#include <TrustWalletCore/TWBitcoinTransactionSigner.h>

#include "../TWNapi.h"
#include "BitcoinTransactionSigner.h"

using namespace Napi;

External<TWBitcoinTransactionSigner> Js_wallet_core_napi_BitcoinTransactionSigner_create(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    struct TWBitcoinTransactionSigner *instance = TWBitcoinTransactionSignerCreate(inputData);
    return External<TWBitcoinTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinTransactionSignerDelete(reinterpret_cast<TWBitcoinTransactionSigner *>(data));
    });
}

External<TWBitcoinTransactionSigner> Js_wallet_core_napi_BitcoinTransactionSigner_createWithPlan(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array inputByteArray = info[0].As<Uint8Array>();
    TWData *inputData = TWDataCreateWithUint8Array(inputByteArray);
    Uint8Array planByteArray = info[1].As<Uint8Array>();
    TWData *planData = TWDataCreateWithUint8Array(planByteArray);
    struct TWBitcoinTransactionSigner *instance = TWBitcoinTransactionSignerCreateWithPlan(inputData, planData);
    return External<TWBitcoinTransactionSigner>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinTransactionSignerDelete(reinterpret_cast<TWBitcoinTransactionSigner *>(data));
    });
}

void Js_wallet_core_napi_BitcoinTransactionSigner_delete(const CallbackInfo& info) {
    TWBitcoinTransactionSignerDelete(info[0].As<External<TWBitcoinTransactionSigner>>().Data());
}

Uint8Array Js_wallet_core_napi_BitcoinTransactionSigner_plan(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinTransactionSigner *instance = info[0].As<External<TWBitcoinTransactionSigner>>().Data();

    TWData* twResult = TWBitcoinTransactionSignerPlan(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinTransactionSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinTransactionSigner *instance = info[0].As<External<TWBitcoinTransactionSigner>>().Data();

    TWData* twResult = TWBitcoinTransactionSignerSign(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

