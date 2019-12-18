// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_WANCHAINADDRESS_H
#define NAPI_TW_WANCHAINADDRESS_H

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWWanchainAddress.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithString(const CallbackInfo& info);

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithKeyHash(const CallbackInfo& info);

External<TWWanchainAddress> Js_wallet_core_napi_WanchainAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_WanchainAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_WanchainAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_WanchainAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_WanchainAddress_description(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_WanchainAddress_keyHash(const CallbackInfo& info);


#endif // NAPI_TW_WANCHAINADDRESS_H