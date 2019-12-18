// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_BITCOINSCRIPT_H
#define NAPI_TW_BITCOINSCRIPT_H

#include <TrustWalletCore/TWBitcoinScript.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_create(const CallbackInfo& info);

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_createWithData(const CallbackInfo& info);

External<TWBitcoinScript> Js_wallet_core_napi_BitcoinScript_createCopy(const CallbackInfo& info);

void Js_wallet_core_napi_BitcoinScript_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinScript_equals(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinScript_buildPayToPublicKeyHash(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinScript_buildPayToScriptHash(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinScript_buildPayToWitnessPubkeyHash(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinScript_buildPayToWitnessScriptHash(const CallbackInfo& info);

Value Js_wallet_core_napi_BitcoinScript_buildForAddress(const CallbackInfo& info);

String Js_wallet_core_napi_BitcoinScript_size(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_data(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_scriptHash(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinScript_isPayToScriptHash(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinScript_isPayToWitnessScriptHash(const CallbackInfo& info);

Boolean Js_wallet_core_napi_BitcoinScript_isWitnessProgram(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToPubkey(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToPubkeyHash(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToScriptHash(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToWitnessPublicKeyHash(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_matchPayToWitnessScriptHash(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_BitcoinScript_encode(const CallbackInfo& info);


#endif // NAPI_TW_BITCOINSCRIPT_H