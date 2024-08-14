// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWPrivateKey.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// TON message signing.
TW_EXPORT_CLASS
struct TWTONMessageSigner;

/// Signs an arbitrary message to prove ownership of an address for off-chain services.
/// https://github.com/ton-foundation/specs/blob/main/specs/wtf-0002.md
///
/// \param privateKey: the private key used for signing
/// \param message: A custom message which is input to the signing.
/// \returns the signature, Hex-encoded. On invalid input null is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONMessageSignerSignMessage(struct TWPrivateKey *_Nonnull privateKey, TWString* _Nonnull message);

TW_EXTERN_C_END
