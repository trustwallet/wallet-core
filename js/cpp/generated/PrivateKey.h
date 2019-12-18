// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_PRIVATEKEY_H
#define NAPI_TW_PRIVATEKEY_H

#include <TrustWalletCore/TWPrivateKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_create(const CallbackInfo& info);

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_createWithData(const CallbackInfo& info);

External<TWPrivateKey> Js_wallet_core_napi_PrivateKey_createCopy(const CallbackInfo& info);

void Js_wallet_core_napi_PrivateKey_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_PrivateKey_isValid(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PrivateKey_data(const CallbackInfo& info);

Value Js_wallet_core_napi_PrivateKey_getPublicKeySecp256k1(const CallbackInfo& info);

Value Js_wallet_core_napi_PrivateKey_getPublicKeyNist256p1(const CallbackInfo& info);

Value Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519(const CallbackInfo& info);

Value Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519Blake2b(const CallbackInfo& info);

Value Js_wallet_core_napi_PrivateKey_getPublicKeyCurve25519(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PrivateKey_sign(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PrivateKey_signAsDER(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PrivateKey_signSchnorr(const CallbackInfo& info);


#endif // NAPI_TW_PRIVATEKEY_H