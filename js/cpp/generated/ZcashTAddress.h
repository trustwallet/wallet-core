// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_ZCASHTADDRESS_H
#define NAPI_TW_ZCASHTADDRESS_H

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWZcashTAddress.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithString(const CallbackInfo& info);

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithData(const CallbackInfo& info);

External<TWZcashTAddress> Js_wallet_core_napi_ZcashTAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_ZcashTAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_ZcashTAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_ZcashTAddress_isValid(const CallbackInfo& info);

Boolean Js_wallet_core_napi_ZcashTAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_ZcashTAddress_description(const CallbackInfo& info);


#endif // NAPI_TW_ZCASHTADDRESS_H