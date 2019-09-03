// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_BITCOINCASHADDRESS_H
#define NAPI_TW_BITCOINCASHADDRESS_H

#include <TrustWalletCore/TWBitcoinCashAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

Value Js_wallet_core_napi_BitcoinCashAddress_initWithString(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinCashAddress_initWithData(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinCashAddress_initWithPublicKey(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinCashAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinCashAddress_isValid(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinCashAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_BitcoinCashAddress_description(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinCashAddress_legacyAddress(const CallbackInfo& info);


#endif // NAPI_TW_BITCOINCASHADDRESS_H