// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_SEMUXADDRESS_H
#define NAPI_TW_SEMUXADDRESS_H

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSemuxAddress.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWSemuxAddress> Js_wallet_core_napi_SemuxAddress_createWithString(const CallbackInfo& info);

External<TWSemuxAddress> Js_wallet_core_napi_SemuxAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_SemuxAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_SemuxAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_SemuxAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_SemuxAddress_description(const CallbackInfo& info);


#endif // NAPI_TW_SEMUXADDRESS_H