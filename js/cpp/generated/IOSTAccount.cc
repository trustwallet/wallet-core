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

#include <TrustWalletCore/TWIOSTAccount.h>

#include "../TWNapi.h"
#include "IOSTAccount.h"

using namespace Napi;

External<TWIOSTAccount> Js_wallet_core_napi_IOSTAccount_createWithString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWIOSTAccount *instance = TWIOSTAccountCreateWithString(stringString);
    TWStringDelete(stringString);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWIOSTAccount>::New(env, instance, [](Env _env, void* data) {
        TWIOSTAccountDelete(reinterpret_cast<TWIOSTAccount *>(data));
    });
}

void Js_wallet_core_napi_IOSTAccount_delete(const CallbackInfo& info) {
    TWIOSTAccountDelete(info[0].As<External<TWIOSTAccount>>().Data());
}

Boolean Js_wallet_core_napi_IOSTAccount_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIOSTAccount *lhsInstance = info[0].As<External<TWIOSTAccount>>().Data();
    struct TWIOSTAccount *rhsInstance = info[1].As<External<TWIOSTAccount>>().Data();
    Boolean resultValue = Boolean::New(env, TWIOSTAccountEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Boolean Js_wallet_core_napi_IOSTAccount_isValidString(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *stringString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWIOSTAccountIsValidString(stringString));
    TWStringDelete(stringString);
    return resultValue;
}

String Js_wallet_core_napi_IOSTAccount_description(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWIOSTAccount *instance = info[0].As<External<TWIOSTAccount>>().Data();

    TWString* twResult = TWIOSTAccountDescription(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

