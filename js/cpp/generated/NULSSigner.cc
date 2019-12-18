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

#include <TrustWalletCore/TWNULSSigner.h>

#include "../TWNapi.h"
#include "NULSSigner.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_NULSSigner_plan(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array purposeByteArray = info[0].As<Uint8Array>();
    TWData *purposeData = TWDataCreateWithUint8Array(purposeByteArray);
    TWData* twResult = TWNULSSignerPlan(purposeData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_NULSSigner_sign(const CallbackInfo& info) {
    Env env = info.Env();
    Uint8Array planByteArray = info[0].As<Uint8Array>();
    TWData *planData = TWDataCreateWithUint8Array(planByteArray);
    TWData* twResult = TWNULSSignerSign(planData);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

