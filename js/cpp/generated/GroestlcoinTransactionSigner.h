// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_GROESTLCOINTRANSACTIONSIGNER_H
#define NAPI_TW_GROESTLCOINTRANSACTIONSIGNER_H

#include <TrustWalletCore/TWGroestlcoinTransactionSigner.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWGroestlcoinTransactionSigner> Js_wallet_core_napi_GroestlcoinTransactionSigner_create(const CallbackInfo& info);

External<TWGroestlcoinTransactionSigner> Js_wallet_core_napi_GroestlcoinTransactionSigner_createWithPlan(const CallbackInfo& info);

void Js_wallet_core_napi_GroestlcoinTransactionSigner_delete(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_GroestlcoinTransactionSigner_plan(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_GroestlcoinTransactionSigner_sign(const CallbackInfo& info);


#endif // NAPI_TW_GROESTLCOINTRANSACTIONSIGNER_H