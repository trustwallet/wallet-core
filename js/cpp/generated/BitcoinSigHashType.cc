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

#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include "../TWNapi.h"
#include "BitcoinSigHashType.h"

using namespace Napi;

Boolean Js_wallet_core_napi_BitcoinSigHashType_isSingle(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWBitcoinSigHashType instance = (enum TWBitcoinSigHashType) ((int) info[0].As<Number>());

    Boolean resultValue = Boolean::New(env, TWBitcoinSigHashTypeIsSingle(instance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinSigHashType_isNone(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWBitcoinSigHashType instance = (enum TWBitcoinSigHashType) ((int) info[0].As<Number>());

    Boolean resultValue = Boolean::New(env, TWBitcoinSigHashTypeIsNone(instance));
    return resultValue;
}

