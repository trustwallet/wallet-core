// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_PKCS8_H
#define NAPI_TW_PKCS8_H

#include <TrustWalletCore/TWPKCS8.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_PKCS8_encodeED25519PrivateKey(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_PKCS8_decodeED25519PrivateKey(const CallbackInfo& info);


#endif // NAPI_TW_PKCS8_H