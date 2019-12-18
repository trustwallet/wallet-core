// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_ZILLIQAADDRESS_H
#define NAPI_TW_ZILLIQAADDRESS_H

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWZilliqaAddress.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithString(const CallbackInfo& info);

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithKeyHash(const CallbackInfo& info);

External<TWZilliqaAddress> Js_wallet_core_napi_ZilliqaAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_ZilliqaAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_ZilliqaAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_ZilliqaAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_ZilliqaAddress_description(const CallbackInfo& info);

String Js_wallet_core_napi_ZilliqaAddress_keyHash(const CallbackInfo& info);


#endif // NAPI_TW_ZILLIQAADDRESS_H