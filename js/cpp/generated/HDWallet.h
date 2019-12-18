// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_HDWALLET_H
#define NAPI_TW_HDWALLET_H

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWHDWallet> Js_wallet_core_napi_HDWallet_create(const CallbackInfo& info);

External<TWHDWallet> Js_wallet_core_napi_HDWallet_createWithMnemonic(const CallbackInfo& info);

External<TWHDWallet> Js_wallet_core_napi_HDWallet_createWithData(const CallbackInfo& info);

void Js_wallet_core_napi_HDWallet_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_HDWallet_isValid(const CallbackInfo& info);

Value Js_wallet_core_napi_HDWallet_getPublicKeyFromExtended(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_HDWallet_seed(const CallbackInfo& info);

String Js_wallet_core_napi_HDWallet_mnemonic(const CallbackInfo& info);

Value Js_wallet_core_napi_HDWallet_getKeyForCoin(const CallbackInfo& info);

Value Js_wallet_core_napi_HDWallet_getKey(const CallbackInfo& info);

Value Js_wallet_core_napi_HDWallet_getKeyBIP44(const CallbackInfo& info);

String Js_wallet_core_napi_HDWallet_getExtendedPrivateKey(const CallbackInfo& info);

String Js_wallet_core_napi_HDWallet_getExtendedPublicKey(const CallbackInfo& info);


#endif // NAPI_TW_HDWALLET_H