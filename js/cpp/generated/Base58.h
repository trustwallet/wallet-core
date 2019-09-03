// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_BASE58_H
#define NAPI_TW_BASE58_H

#include <TrustWalletCore/TWBase58.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

String Js_wallet_core_napi_Base58_encode(const CallbackInfo& info);

String Js_wallet_core_napi_Base58_encodeNoCheck(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Base58_decode(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Base58_decodeNoCheck(const CallbackInfo& info);


#endif // NAPI_TW_BASE58_H