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

#include <TrustWalletCore/TWStoredKey.h>

#include "../TWNapi.h"
#include "StoredKey.h"

using namespace Napi;

External<TWStoredKey> Js_wallet_core_napi_StoredKey_create(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *nameString = TWStringCreateWithJsString(info[0].As<String>());
    TWString *passwordString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWStoredKey *instance = TWStoredKeyCreate(nameString, passwordString);
    TWStringDelete(nameString);
    TWStringDelete(passwordString);
    return External<TWStoredKey>::New(env, instance, [](Env _env, void* data) {
        TWStoredKeyDelete(reinterpret_cast<TWStoredKey *>(data));
    });
}

void Js_wallet_core_napi_StoredKey_delete(const CallbackInfo& info) {
    TWStoredKeyDelete(info[0].As<External<TWStoredKey>>().Data());
}

Value Js_wallet_core_napi_StoredKey_load(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *pathString = TWStringCreateWithJsString(info[0].As<String>());
    struct TWStoredKey *result = TWStoredKeyLoad(pathString);
    TWStringDelete(pathString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWStoredKey>::New(env, result, [](Env _env, void* data) {
        TWStoredKeyDelete(reinterpret_cast<TWStoredKey *>(data));
    });
}

Value Js_wallet_core_napi_StoredKey_importPrivateKey(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *privateKeyData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWString *nameString = TWStringCreateWithJsString(info[1].As<String>());
    TWString *passwordString = TWStringCreateWithJsString(info[2].As<String>());
    TWCoinType coinValue = (TWCoinType)((int) info[3].As<Number>());
    struct TWStoredKey *result = TWStoredKeyImportPrivateKey(privateKeyData, nameString, passwordString, coinValue);
    TWDataDelete(privateKeyData);
    TWStringDelete(nameString);
    TWStringDelete(passwordString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWStoredKey>::New(env, result, [](Env _env, void* data) {
        TWStoredKeyDelete(reinterpret_cast<TWStoredKey *>(data));
    });
}

Value Js_wallet_core_napi_StoredKey_importHDWallet(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *mnemonicString = TWStringCreateWithJsString(info[0].As<String>());
    TWString *nameString = TWStringCreateWithJsString(info[1].As<String>());
    TWString *passwordString = TWStringCreateWithJsString(info[2].As<String>());
    TWCoinType coinValue = (TWCoinType)((int) info[3].As<Number>());
    struct TWStoredKey *result = TWStoredKeyImportHDWallet(mnemonicString, nameString, passwordString, coinValue);
    TWStringDelete(mnemonicString);
    TWStringDelete(nameString);
    TWStringDelete(passwordString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWStoredKey>::New(env, result, [](Env _env, void* data) {
        TWStoredKeyDelete(reinterpret_cast<TWStoredKey *>(data));
    });
}

Value Js_wallet_core_napi_StoredKey_importJSON(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *jsonData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    struct TWStoredKey *result = TWStoredKeyImportJSON(jsonData);
    TWDataDelete(jsonData);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWStoredKey>::New(env, result, [](Env _env, void* data) {
        TWStoredKeyDelete(reinterpret_cast<TWStoredKey *>(data));
    });
}

String Js_wallet_core_napi_StoredKey_identifier(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString* twResult = TWStoredKeyIdentifier(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_StoredKey_name(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString* twResult = TWStoredKeyName(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Boolean Js_wallet_core_napi_StoredKey_isMnemonic(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    Boolean resultValue = Boolean::New(env, TWStoredKeyIsMnemonic(instance));
    return resultValue;
}

String Js_wallet_core_napi_StoredKey_accountCount(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    String returnValue = String::New(env, std::to_string(TWStoredKeyAccountCount(instance)));
    return returnValue;
}

Value Js_wallet_core_napi_StoredKey_account(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    size_t index = std::stoull((std::string) info[1].As<String>());
    struct TWAccount *result = TWStoredKeyAccount(instance, index);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWAccount>::New(env, result, [](Env _env, void* data) {
        TWAccountDelete(reinterpret_cast<TWAccount *>(data));
    });
}

Value Js_wallet_core_napi_StoredKey_accountForCoin(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWCoinType coinValue = (TWCoinType)((int) info[1].As<Number>());
    struct TWHDWallet *walletInstance = info[2].As<External<TWHDWallet>>().Data();
    struct TWAccount *result = TWStoredKeyAccountForCoin(instance, coinValue, walletInstance);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWAccount>::New(env, result, [](Env _env, void* data) {
        TWAccountDelete(reinterpret_cast<TWAccount *>(data));
    });
}

void Js_wallet_core_napi_StoredKey_addAccount(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *addressString = TWStringCreateWithJsString(info[1].As<String>());
    TWString *derivationPathString = TWStringCreateWithJsString(info[2].As<String>());
    TWString *extetndedPublicKeyString = TWStringCreateWithJsString(info[3].As<String>());
    TWStoredKeyAddAccount(instance, addressString, derivationPathString, extetndedPublicKeyString);
    TWStringDelete(addressString);
    TWStringDelete(derivationPathString);
    TWStringDelete(extetndedPublicKeyString);
}

Boolean Js_wallet_core_napi_StoredKey_store(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *pathString = TWStringCreateWithJsString(info[1].As<String>());
    Boolean resultValue = Boolean::New(env, TWStoredKeyStore(instance, pathString));
    TWStringDelete(pathString);
    return resultValue;
}

Uint8Array Js_wallet_core_napi_StoredKey_decryptPrivateKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *passwordString = TWStringCreateWithJsString(info[1].As<String>());
    TWData* twResult = TWStoredKeyDecryptPrivateKey(instance, passwordString);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    TWStringDelete(passwordString);
    return result;
}

String Js_wallet_core_napi_StoredKey_decryptMnemonic(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *passwordString = TWStringCreateWithJsString(info[1].As<String>());
    TWString* twResult = TWStoredKeyDecryptMnemonic(instance, passwordString);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    String result = TWStringJsString(&env, twResult);
    TWStringDelete(passwordString);
    return result;
}

Value Js_wallet_core_napi_StoredKey_privateKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWCoinType coinValue = (TWCoinType)((int) info[1].As<Number>());
    TWString *passwordString = TWStringCreateWithJsString(info[2].As<String>());
    struct TWPrivateKey *result = TWStoredKeyPrivateKey(instance, coinValue, passwordString);
    TWStringDelete(passwordString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPrivateKey>::New(env, result, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

Value Js_wallet_core_napi_StoredKey_wallet(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *passwordString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWHDWallet *result = TWStoredKeyWallet(instance, passwordString);
    TWStringDelete(passwordString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWHDWallet>::New(env, result, [](Env _env, void* data) {
        TWHDWalletDelete(reinterpret_cast<TWHDWallet *>(data));
    });
}

Uint8Array Js_wallet_core_napi_StoredKey_exportJSON(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWData* twResult = TWStoredKeyExportJSON(instance);
    if (twResult == nullptr) {
        Error::New(env, "Invalid input").ThrowAsJavaScriptException();
    }
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

Boolean Js_wallet_core_napi_StoredKey_fixAddresses(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWStoredKey *instance = info[0].As<External<TWStoredKey>>().Data();

    TWString *passwordString = TWStringCreateWithJsString(info[1].As<String>());
    Boolean resultValue = Boolean::New(env, TWStoredKeyFixAddresses(instance, passwordString));
    TWStringDelete(passwordString);
    return resultValue;
}

