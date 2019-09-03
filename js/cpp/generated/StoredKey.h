// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_STOREDKEY_H
#define NAPI_TW_STOREDKEY_H

#include <TrustWalletCore/TWStoredKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWStoredKey> Js_wallet_core_napi_StoredKey_create(const CallbackInfo& info);

void Js_wallet_core_napi_StoredKey_delete(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_load(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_importPrivateKey(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_importHDWallet(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_importJSON(const CallbackInfo& info);

String Js_wallet_core_napi_StoredKey_identifier(const CallbackInfo& info);

String Js_wallet_core_napi_StoredKey_name(const CallbackInfo& info);

Boolean Js_wallet_core_napi_StoredKey_isMnemonic(const CallbackInfo& info);

String Js_wallet_core_napi_StoredKey_accountCount(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_account(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_accountForCoin(const CallbackInfo& info);

void Js_wallet_core_napi_StoredKey_addAccount(const CallbackInfo& info);

Boolean Js_wallet_core_napi_StoredKey_store(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_StoredKey_decryptPrivateKey(const CallbackInfo& info);

String Js_wallet_core_napi_StoredKey_decryptMnemonic(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_privateKey(const CallbackInfo& info);

Value Js_wallet_core_napi_StoredKey_wallet(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_StoredKey_exportJSON(const CallbackInfo& info);

Boolean Js_wallet_core_napi_StoredKey_fixAddresses(const CallbackInfo& info);


#endif // NAPI_TW_STOREDKEY_H