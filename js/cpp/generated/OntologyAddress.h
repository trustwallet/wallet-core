// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef NAPI_TW_ONTOLOGYADDRESS_H
#define NAPI_TW_ONTOLOGYADDRESS_H

#include <TrustWalletCore/TWOntologyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBase.h>
#include "../TWNapi.h"

using namespace Napi;

External<TWOntologyAddress> Js_wallet_core_napi_OntologyAddress_createWithString(const CallbackInfo& info);

External<TWOntologyAddress> Js_wallet_core_napi_OntologyAddress_createWithPublicKey(const CallbackInfo& info);

void Js_wallet_core_napi_OntologyAddress_delete(const CallbackInfo& info);

Boolean Js_wallet_core_napi_OntologyAddress_equals(const CallbackInfo& info);

Boolean Js_wallet_core_napi_OntologyAddress_isValidString(const CallbackInfo& info);

String Js_wallet_core_napi_OntologyAddress_description(const CallbackInfo& info);

Uint8Array Js_wallet_core_napi_OntologyAddress_keyHash(const CallbackInfo& info);


#endif // NAPI_TW_ONTOLOGYADDRESS_H