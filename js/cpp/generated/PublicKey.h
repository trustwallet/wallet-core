// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_PUBLICKEY_H
#define NAPI_TW_PUBLICKEY_H

#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWPublicKey> Js_wallet_core_napi_PublicKey_createWithData(const CallbackInfo& info);

void Js_wallet_core_napi_PublicKey_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_PublicKey_isValid(const CallbackInfo& info);

Value Js_wallet_core_napi_PublicKey_recover(const CallbackInfo& info);

Boolean Js_wallet_core_napi_PublicKey_isCompressed(const CallbackInfo& info);

Value Js_wallet_core_napi_PublicKey_compressed(const CallbackInfo& info);

Value Js_wallet_core_napi_PublicKey_uncompressed(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PublicKey_data(const CallbackInfo& info);

Number Js_wallet_core_napi_PublicKey_keyType(const CallbackInfo& info);

String Js_wallet_core_napi_PublicKey_description(const CallbackInfo& info);

Boolean Js_wallet_core_napi_PublicKey_verify(const CallbackInfo& info);

Boolean Js_wallet_core_napi_PublicKey_verifySchnorr(const CallbackInfo& info);


#endif // NAPI_TW_PUBLICKEY_H