// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_BITCOINADDRESS_H
#define NAPI_TW_BITCOINADDRESS_H

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithString(const CallbackInfo& info);

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithData(const CallbackInfo& info);

External<TWBitcoinAddress> Js_wallet_core_napi_BitcoinAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_BitcoinAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinAddress_isValid(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_BitcoinAddress_description(const CallbackInfo& info);

Number Js_wallet_core_napi_BitcoinAddress_prefix(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinAddress_keyhash(const CallbackInfo& info);


#endif // NAPI_TW_BITCOINADDRESS_H