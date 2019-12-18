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

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "../TWNapi.h"
#include "HDWallet.h"

using namespace Napi;

External<TWHDWallet> Js_wallet_core_napi_HDWallet_create(const CallbackInfo& info) {
    Env env = info.Env();
    int strength = info[0].As<Number>().Int64Value();
    TWString *passphraseString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWHDWallet *instance = TWHDWalletCreate(strength, passphraseString);
    TWStringDelete(passphraseString);
    return External<TWHDWallet>::New(env, instance, [](Env _env, void* data) {
        TWHDWalletDelete(reinterpret_cast<TWHDWallet *>(data));
    });
}

External<TWHDWallet> Js_wallet_core_napi_HDWallet_createWithMnemonic(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *mnemonicString = TWStringCreateWithJsString(info[0].As<String>());
    TWString *passphraseString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWHDWallet *instance = TWHDWalletCreateWithMnemonic(mnemonicString, passphraseString);
    TWStringDelete(mnemonicString);
    TWStringDelete(passphraseString);
    return External<TWHDWallet>::New(env, instance, [](Env _env, void* data) {
        TWHDWalletDelete(reinterpret_cast<TWHDWallet *>(data));
    });
}

External<TWHDWallet> Js_wallet_core_napi_HDWallet_createWithData(const CallbackInfo& info) {
    Env env = info.Env();
    TWData *dataData = TWDataCreateWithUint8Array(info[0].As<Uint8Array>());
    TWString *passphraseString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWHDWallet *instance = TWHDWalletCreateWithData(dataData, passphraseString);
    TWDataDelete(dataData);
    TWStringDelete(passphraseString);
    return External<TWHDWallet>::New(env, instance, [](Env _env, void* data) {
        TWHDWalletDelete(reinterpret_cast<TWHDWallet *>(data));
    });
}

void Js_wallet_core_napi_HDWallet_delete(const CallbackInfo& info) {
    TWHDWalletDelete(info[0].As<External<TWHDWallet>>().Data());
}

Boolean Js_wallet_core_napi_HDWallet_isValid(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *mnemonicString = TWStringCreateWithJsString(info[0].As<String>());
    Boolean resultValue = Boolean::New(env, TWHDWalletIsValid(mnemonicString));
    TWStringDelete(mnemonicString);
    return resultValue;
}

Value Js_wallet_core_napi_HDWallet_getPublicKeyFromExtended(const CallbackInfo& info) {
    Env env = info.Env();
    TWString *extendedString = TWStringCreateWithJsString(info[0].As<String>());
    TWString *derivationPathString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWPublicKey *result = TWHDWalletGetPublicKeyFromExtended(extendedString, derivationPathString);
    TWStringDelete(extendedString);
    TWStringDelete(derivationPathString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPublicKey>::New(env, result, [](Env _env, void* data) {
        TWPublicKeyDelete(reinterpret_cast<TWPublicKey *>(data));
    });
}

Uint8Array Js_wallet_core_napi_HDWallet_seed(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWData* twResult = TWHDWalletSeed(instance);
    Uint8Array result = TWDataUint8Array(&env, twResult);
    return result;
}

String Js_wallet_core_napi_HDWallet_mnemonic(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWString* twResult = TWHDWalletMnemonic(instance);
    String result = TWStringJsString(&env, twResult);
    return result;
}

Value Js_wallet_core_napi_HDWallet_getKeyForCoin(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWCoinType coinValue = (TWCoinType)((int) info[1].As<Number>());
    struct TWPrivateKey *result = TWHDWalletGetKeyForCoin(instance, coinValue);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPrivateKey>::New(env, result, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

Value Js_wallet_core_napi_HDWallet_getKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWString *derivationPathString = TWStringCreateWithJsString(info[1].As<String>());
    struct TWPrivateKey *result = TWHDWalletGetKey(instance, derivationPathString);
    TWStringDelete(derivationPathString);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPrivateKey>::New(env, result, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

Value Js_wallet_core_napi_HDWallet_getKeyBIP44(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWCoinType coinValue = (TWCoinType)((int) info[1].As<Number>());
    uint32_t account = info[2].As<Number>().Int64Value();
    uint32_t change = info[3].As<Number>().Int64Value();
    uint32_t address = info[4].As<Number>().Int64Value();
    struct TWPrivateKey *result = TWHDWalletGetKeyBIP44(instance, coinValue, account, change, address);
    if (result == nullptr) {
        return env.Null();
    }

    return External<TWPrivateKey>::New(env, result, [](Env _env, void* data) {
        TWPrivateKeyDelete(reinterpret_cast<TWPrivateKey *>(data));
    });
}

String Js_wallet_core_napi_HDWallet_getExtendedPrivateKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWPurpose purposeValue = (TWPurpose)((int) info[1].As<Number>());
    TWCoinType coinValue = (TWCoinType)((int) info[2].As<Number>());
    TWHDVersion versionValue = (TWHDVersion)((int) info[3].As<Number>());
    TWString* twResult = TWHDWalletGetExtendedPrivateKey(instance, purposeValue, coinValue, versionValue);
    String result = TWStringJsString(&env, twResult);
    return result;
}

String Js_wallet_core_napi_HDWallet_getExtendedPublicKey(const CallbackInfo& info) {
    Env env = info.Env();
    struct TWHDWallet *instance = info[0].As<External<TWHDWallet>>().Data();

    TWPurpose purposeValue = (TWPurpose)((int) info[1].As<Number>());
    TWCoinType coinValue = (TWCoinType)((int) info[2].As<Number>());
    TWHDVersion versionValue = (TWHDVersion)((int) info[3].As<Number>());
    TWString* twResult = TWHDWalletGetExtendedPublicKey(instance, purposeValue, coinValue, versionValue);
    String result = TWStringJsString(&env, twResult);
    return result;
}

