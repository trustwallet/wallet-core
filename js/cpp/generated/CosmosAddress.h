// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_COSMOSADDRESS_H
#define NAPI_TW_COSMOSADDRESS_H

#include <TrustWalletCore/TWCosmosAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithString(const CallbackInfo& info);

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithKeyHash(const CallbackInfo& info);

External<TWCosmosAddress> Js_wallet_core_napi_CosmosAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_CosmosAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_CosmosAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_CosmosAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_CosmosAddress_description(const CallbackInfo& info);

Number Js_wallet_core_napi_CosmosAddress_hrp(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_CosmosAddress_keyHash(const CallbackInfo& info);


#endif // NAPI_TW_COSMOSADDRESS_H