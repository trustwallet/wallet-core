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

#include <TrustWalletCore/TWBitcoinScript.h>

#include "../TWNapi.h"
#include "BitcoinScript.h"

using namespace Napi;

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_create(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = TWBitcoinScriptCreate();
    return External<TWBitcoinScript>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinScript *instance = TWBitcoinScriptCreateWithData(dataData);
    TWDataDelete(dataData);
    return External<TWBitcoinScript>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_createCopy(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *scriptInstance = info[0].As<External<TWBitcoinScript>>().Data();
    struct TWBitcoinScript *instance = TWBitcoinScriptCreateCopy(scriptInstance);
    return External<TWBitcoinScript>::New(env, instance, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

void Js_wallet_core_napi_BitcoinScript_delete(const CallbackInfo& info) {
    TWBitcoinScriptDelete(info[0].As<External<TWBitcoinScript>>().Data());
}

Boolean Js_wallet_core_napi_BitcoinScript_equals(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *lhsInstance = info[0].As<External<TWBitcoinScript>>().Data();
    struct TWBitcoinScript *rhsInstance = info[1].As<External<TWBitcoinScript>>().Data();
    Boolean resultValue = Boolean::New(env, TWBitcoinScriptEqual(lhsInstance, rhsInstance));
    return resultValue;
}

Value Js_wallet_core_napi_BitcoinScript_buildPayToPublicKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *hashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinScript *result = TWBitcoinScriptBuildPayToPublicKeyHash(hashData);
    TWDataDelete(hashData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinScript>::New(env, result, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

Value Js_wallet_core_napi_BitcoinScript_buildPayToScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *scriptHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinScript *result = TWBitcoinScriptBuildPayToScriptHash(scriptHashData);
    TWDataDelete(scriptHashData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinScript>::New(env, result, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

Value Js_wallet_core_napi_BitcoinScript_buildPayToWitnessPubkeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *hashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinScript *result = TWBitcoinScriptBuildPayToWitnessPubkeyHash(hashData);
    TWDataDelete(hashData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinScript>::New(env, result, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

Value Js_wallet_core_napi_BitcoinScript_buildPayToWitnessScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *scriptHashData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWBitcoinScript *result = TWBitcoinScriptBuildPayToWitnessScriptHash(scriptHashData);
    TWDataDelete(scriptHashData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinScript>::New(env, result, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

Value Js_wallet_core_napi_BitcoinScript_buildForAddress(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *addressString = TWStringCreateWithJsString(info[0].As<String>());
    TWCoinType coinValue = (TWCoinType)((int) info[1].As<Number>());
    struct TWBitcoinScript *result = TWBitcoinScriptBuildForAddress(addressString, coinValue);
    TWStringDelete(addressString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWBitcoinScript>::New(env, result, [](Env _env, void* data) {
        TWBitcoinScriptDelete(reinterpret_cast<TWBitcoinScript *>(data));
    });
}

String Js_wallet_core_napi_BitcoinScript_size(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    String returnValue = String::New(env, std::to_string(TWBitcoinScriptSize(instance)));
    return returnValue;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_data(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptData(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_scriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptScriptHash(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Boolean Js_wallet_core_napi_BitcoinScript_isPayToScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    Boolean resultValue = Boolean::New(env, TWBitcoinScriptIsPayToScriptHash(instance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinScript_isPayToWitnessScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    Boolean resultValue = Boolean::New(env, TWBitcoinScriptIsPayToWitnessScriptHash(instance));
    return resultValue;
}

Boolean Js_wallet_core_napi_BitcoinScript_isWitnessProgram(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    Boolean resultValue = Boolean::New(env, TWBitcoinScriptIsWitnessProgram(instance));
    return resultValue;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToPubkey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptMatchPayToPubkey(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToPubkeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptMatchPayToPubkeyHash(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptMatchPayToScriptHash(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToWitnessPublicKeyHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptMatchPayToWitnessPublicKeyHash(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToWitnessScriptHash(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptMatchPayToWitnessScriptHash(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Uint8Array Js_wallet_core_napi_BitcoinScript_encode(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWBitcoinScript *instance = info[0].As<External<TWBitcoinScript>>().Data();

    TWData* twResult = TWBitcoinScriptEncode(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

