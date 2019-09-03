// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_COINTYPE_H
#define NAPI_TW_COINTYPE_H

#include <TrustWalletCore/TWCoinType.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

Number Js_wallet_core_napi_CoinType_blockchain(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_purpose(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_curve(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_xpubVersion(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_xprvVersion(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_hrp(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_p2pkhPrefix(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_p2shPrefix(const CallbackInfo& info);

Number Js_wallet_core_napi_CoinType_staticPrefix(const CallbackInfo& info);

Boolean Js_wallet_core_napi_CoinType_validate(const CallbackInfo& info);

String Js_wallet_core_napi_CoinType_derivationPath(const CallbackInfo& info);

String Js_wallet_core_napi_CoinType_deriveAddress(const CallbackInfo& info);

String Js_wallet_core_napi_CoinType_deriveAddressFromPublicKey(const CallbackInfo& info);


#endif // NAPI_TW_COINTYPE_H