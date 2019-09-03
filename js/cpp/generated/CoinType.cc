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

#include <TrustWalletCore/TWCoinType.h>

#include "../TWNapi.h"
#include "CoinType.h"

using namespace Napi;

Number Js_wallet_core_napi_CoinType_blockchain(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWBlockchain result = TWCoinTypeBlockchain(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_purpose(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWPurpose result = TWCoinTypePurpose(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_curve(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWCurve result = TWCoinTypeCurve(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_xpubVersion(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWHDVersion result = TWCoinTypeXpubVersion(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_xprvVersion(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWHDVersion result = TWCoinTypeXprvVersion(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_hrp(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    enum TWHRP result = TWCoinTypeHRP(instance);
    return Number::New(env, (int) result);;
}

Number Js_wallet_core_napi_CoinType_p2pkhPrefix(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    Number resultValue = Number::New(env, TWCoinTypeP2pkhPrefix(instance));
    return resultValue;
}

Number Js_wallet_core_napi_CoinType_p2shPrefix(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    Number resultValue = Number::New(env, TWCoinTypeP2shPrefix(instance));
    return resultValue;
}

Number Js_wallet_core_napi_CoinType_staticPrefix(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    Number resultValue = Number::New(env, TWCoinTypeStaticPrefix(instance));
    return resultValue;
}

Boolean Js_wallet_core_napi_CoinType_validate(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    TWString *addressString = TWStringCreateWithJsString(info[1].As<String>());
    Boolean resultValue = Boolean::New(env, TWCoinTypeValidate(instance, addressString));
    TWStringDelete(addressString);
    return resultValue;
}

String Js_wallet_core_napi_CoinType_derivationPath(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    TWString* twResult = TWCoinTypeDerivationPath(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_CoinType_deriveAddress(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    struct TWPrivateKey *privateKeyInstance = info[1].As<External<TWPrivateKey>>().Data();
    TWString* twResult = TWCoinTypeDeriveAddress(instance, privateKeyInstance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_CoinType_deriveAddressFromPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    enum TWCoinType instance = (enum TWCoinType) ((int) info[0].As<Number>());

    struct TWPublicKey *publicKeyInstance = info[1].As<External<TWPublicKey>>().Data();
    TWString* twResult = TWCoinTypeDeriveAddressFromPublicKey(instance, publicKeyInstance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

