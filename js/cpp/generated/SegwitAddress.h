// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_SEGWITADDRESS_H
#define NAPI_TW_SEGWITADDRESS_H

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSegwitAddress.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWSegwitAddress> Js_wallet_core_napi_SegwitAddress_createWithString(const CallbackInfo& info);

External<TWSegwitAddress> Js_wallet_core_napi_SegwitAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_SegwitAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_SegwitAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_SegwitAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_SegwitAddress_description(const CallbackInfo& info);

Number Js_wallet_core_napi_SegwitAddress_hrp(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_SegwitAddress_witnessProgram(const CallbackInfo& info);


#endif // NAPI_TW_SEGWITADDRESS_H