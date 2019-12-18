// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_DECREDSIGNER_H
#define NAPI_TW_DECREDSIGNER_H

#include <TrustWalletCore/TWDecredSigner.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWDecredSigner> Js_wallet_core_napi_DecredSigner_create(const CallbackInfo& info);

External<TWDecredSigner> Js_wallet_core_napi_DecredSigner_createWithPlan(const CallbackInfo& info);

void Js_wallet_core_napi_DecredSigner_delete(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_DecredSigner_plan(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_DecredSigner_sign(const CallbackInfo& info);


#endif // NAPI_TW_DECREDSIGNER_H