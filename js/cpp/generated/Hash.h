// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_HASH_H
#define NAPI_TW_HASH_H

#include <TrustWalletCore/TWHash.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

Uint8Array Js_wallet_core_napi_Hash_sha1(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha256(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha512(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_keccak256(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_keccak512(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha3256(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha3512(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_ripemd(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_blake2b(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha256RIPEMD(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_Hash_sha256SHA256(const CallbackInfo& info);


#endif // NAPI_TW_HASH_H