// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_COINTYPECONFIGURATION_H
#define NAPI_TW_COINTYPECONFIGURATION_H

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

String Js_wallet_core_napi_CoinTypeConfiguration_getSymbol(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinTypeConfiguration_getDecimals(const CallbackInfo& info);

String Js_wallet_core_napi_CoinTypeConfiguration_getTransactionURL(const CallbackInfo& info);

String Js_wallet_core_napi_CoinTypeConfiguration_getID(const CallbackInfo& info);

String Js_wallet_core_napi_CoinTypeConfiguration_getName(const CallbackInfo& info);


#endif // NAPI_TW_COINTYPECONFIGURATION_H