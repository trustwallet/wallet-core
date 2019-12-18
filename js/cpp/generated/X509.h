// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_X509_H
#define NAPI_TW_X509_H

#include <TrustWalletCore/TWX509.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_X509_encodeED25519PublicKey(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_X509_decodeED25519PublicKey(const CallbackInfo& info);


#endif // NAPI_TW_X509_H