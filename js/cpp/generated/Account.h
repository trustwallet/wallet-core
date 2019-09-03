// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_ACCOUNT_H
#define NAPI_TW_ACCOUNT_H

#include <TrustWalletCore/TWAccount.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWAccount> Js_wallet_core_napi_Account_create(const CallbackInfo& info);

void Js_wallet_core_napi_Account_delete(const CallbackInfo& info);

String Js_wallet_core_napi_Account_address(const CallbackInfo& info);

String Js_wallet_core_napi_Account_derivationPath(const CallbackInfo& info);

String Js_wallet_core_napi_Account_extendedPublicKey(const CallbackInfo& info);

Number Js_wallet_core_napi_Account_coin(const CallbackInfo& info);


#endif // NAPI_TW_ACCOUNT_H