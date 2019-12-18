// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_NEBULASADDRESS_H
#define NAPI_TW_NEBULASADDRESS_H

#include <TrustWalletCore/TWNebulasAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWNebulasAddress> Js_wallet_core_napi_NebulasAddress_createWithString(const CallbackInfo& info);

External<TWNebulasAddress> Js_wallet_core_napi_NebulasAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_NebulasAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_NebulasAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_NebulasAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_NebulasAddress_description(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_NebulasAddress_keyHash(const CallbackInfo& info);


#endif // NAPI_TW_NEBULASADDRESS_H