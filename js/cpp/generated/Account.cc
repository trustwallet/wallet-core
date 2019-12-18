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

#include <TrustWalletCore/TWAccount.h>

#include "../TWNapi.h"
#include "Account.h"

using namespace Napi;

External<TWAccount> Js_wallet_core_napi_Account_create(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *addressString = TWStringCreateWithJsString(info[0].As<String>());
    TWString *derivationPathString = TWStringCreateWithJsString(info[1].As<String>());
    TWString *extendedPublicKeyString = TWStringCreateWithJsString(info[2].As<String>());
    struct TWAccount *instance = TWAccountCreate(addressString, derivationPathString, extendedPublicKeyString);
    TWStringDelete(addressString);
    TWStringDelete(derivationPathString);
    TWStringDelete(extendedPublicKeyString);
    return External<TWAccount>::New(env, instance, [](Env _env, void* data) {
        TWAccountDelete(reinterpret_cast<TWAccount *>(data));
    });
}

void Js_wallet_core_napi_Account_delete(const CallbackInfo& info) {
    TWAccountDelete(info[0].As<External<TWAccount>>().Data());
}

String Js_wallet_core_napi_Account_address(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAccount *instance = info[0].As<External<TWAccount>>().Data();

    TWString* twResult = TWAccountAddress(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_Account_derivationPath(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAccount *instance = info[0].As<External<TWAccount>>().Data();

    TWString* twResult = TWAccountDerivationPath(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_Account_extendedPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAccount *instance = info[0].As<External<TWAccount>>().Data();

    TWString* twResult = TWAccountExtendedPublicKey(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Number Js_wallet_core_napi_Account_coin(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWAccount *instance = info[0].As<External<TWAccount>>().Data();

    enum TWCoinType result = TWAccountCoin(instance);
    return Number::New(env, (int) result);;
}

