// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_BITCOINTRANSACTIONSIGNER_H
#define NAPI_TW_BITCOINTRANSACTIONSIGNER_H

#include <TrustWalletCore/TWBitcoinTransactionSigner.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWBitcoinTransactionSigner> Js_wallet_core_napi_BitcoinTransactionSigner_create(const CallbackInfo& info);

External<TWBitcoinTransactionSigner> Js_wallet_core_napi_BitcoinTransactionSigner_createWithPlan(const CallbackInfo& info);

void Js_wallet_core_napi_BitcoinTransactionSigner_delete(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinTransactionSigner_plan(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinTransactionSigner_sign(const CallbackInfo& info);


#endif // NAPI_TW_BITCOINTRANSACTIONSIGNER_H