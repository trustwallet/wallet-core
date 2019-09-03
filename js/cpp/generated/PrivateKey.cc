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

#include <TrustWalletCore/TWPrivateKey.h>

#include "../TWNapi.h"
#include "PrivateKey.h"

using namespace Napi;

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_create(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = TWPrivateKeyCreate();
    return External<TWPrivateKey>::New(env, instance, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWPrivateKey *instance = TWPrivateKeyCreateWithData(dataData);
    TWDataDelete(dataData);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWPrivateKey>::New(env, instance, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_createCopy(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *keyInstance = info[0].As<External<TWPrivateKey>>().Data();
    struct TWPrivateKey *instance = TWPrivateKeyCreateCopy(keyInstance);
    if (instance == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    return External<TWPrivateKey>::New(env, instance, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

void Js_wallet_core_napi_PrivateKey_delete(const CallbackInfo& info) {
    TWPrivateKeyDelete(info[0].As<External<TWPrivateKey>>().Data());
}

Boolean Js_wallet_core_napi_PrivateKey_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWCurve curveValue = (TWCurve)((int) info[1].As<Number>());
    Boolean resultValue = Boolean::New(env, TWPrivateKeyIsValid(dataData, curveValue));
    TWDataDelete(dataData);
    return resultValue;
}

Uint8Array Js_wallet_core_napi_PrivateKey_data(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    TWData* twResult = TWPrivateKeyData(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Value Js_wallet_core_napi_PrivateKey_getPublicKeySecp256k1(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    bool compressed = (bool) info[1].As<Boolean>();
    struct TWPublicKey *result = TWPrivateKeyGetPublicKeySecp256k1(instance, compressed);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Value Js_wallet_core_napi_PrivateKey_getPublicKeyNist256p1(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeyNist256p1(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Value Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeyEd25519(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Value Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519Blake2b(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeyEd25519Blake2b(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Value Js_wallet_core_napi_PrivateKey_getPublicKeyCurve25519(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeyCurve25519(instance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Uint8Array Js_wallet_core_napi_PrivateKey_sign(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    TWData *digestData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWCurve curveValue = (TWCurve)((int) info[2].As<Number>());
    TWData* twResult = TWPrivateKeySign(instance, digestData, curveValue);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(digestData);
    return result;
}

Uint8Array Js_wallet_core_napi_PrivateKey_signAsDER(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    TWData *digestData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWCurve curveValue = (TWCurve)((int) info[2].As<Number>());
    TWData* twResult = TWPrivateKeySignAsDER(instance, digestData, curveValue);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(digestData);
    return result;
}

Uint8Array Js_wallet_core_napi_PrivateKey_signSchnorr(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWPrivateKey *instance = info[0].As<External<TWPrivateKey>>().Data();

    TWData *messageData = TWDataCreateWithUint8Array(info[1].As<Uint8Array>());
    TWCurve curveValue = (TWCurve)((int) info[2].As<Number>());
    TWData* twResult = TWPrivateKeySignSchnorr(instance, messageData, curveValue);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWDataDelete(messageData);
    return result;
}

