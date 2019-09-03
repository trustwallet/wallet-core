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

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include "../TWNapi.h"
#include "CoinTypeConfiguration.h"

using namespace Napi;

String Js_wallet_core_napi_CoinTypeConfiguration_getSymbol(const CallbackInfo& info) {
    Env env = info.Env();
    TWCoinType typeValue = (TWCoinType)((int) info[0].As<Number>());
    TWString* twResult = TWCoinTypeConfigurationGetSymbol(typeValue);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_CoinTypeConfiguration_getDecimals(const CallbackInfo& info) {
    Env env = info.Env();
    TWCoinType typeValue = (TWCoinType)((int) info[0].As<Number>());
    Number resultValue = Number::New(env, TWCoinTypeConfigurationGetDecimals(typeValue));
    return resultValue;
}

String Js_wallet_core_napi_CoinTypeConfiguration_getTransactionURL(const CallbackInfo& info) {
    Env env = info.Env();
    TWCoinType typeValue = (TWCoinType)((int) info[0].As<Number>());
    TWString *transactionIDString = TWStringCreateWithJsString(info[1].As<String>());
    TWString* twResult = TWCoinTypeConfigurationGetTransactionURL(typeValue, transactionIDString);
    String result = TWStringJsString(&env, twResult);
    TWStringDelete(transactionIDString);
    return result;
}

String Js_wallet_core_napi_CoinTypeConfiguration_getID(const CallbackInfo& info) {
    Env env = info.Env();
    TWCoinType typeValue = (TWCoinType)((int) info[0].As<Number>());
    TWString* twResult = TWCoinTypeConfigurationGetID(typeValue);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_CoinTypeConfiguration_getName(const CallbackInfo& info) {
    Env env = info.Env();
    TWCoinType typeValue = (TWCoinType)((int) info[0].As<Number>());
    TWString* twResult = TWCoinTypeConfigurationGetName(typeValue);
    String result = TWStringJsString(&env, twResult);
    return result;
}

