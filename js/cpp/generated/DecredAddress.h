// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_DECREDADDRESS_H
#define NAPI_TW_DECREDADDRESS_H

#include <TrustWalletCore/TWDecredAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWDecredAddress> Js_wallet_core_napi_DecredAddress_createWithString(const CallbackInfo& info);

External<TWDecredAddress> Js_wallet_core_napi_DecredAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_DecredAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_DecredAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_DecredAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_DecredAddress_description(const CallbackInfo& info);


#endif // NAPI_TW_DECREDADDRESS_H