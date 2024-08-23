// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWPrivateKey.h"

TW_EXTERN_C_BEGIN

/// StarkEx message signing and verification.
///
/// StarkEx and some other wallets support a message signing & verification format, to create a proof (a signature)
/// that someone has access to the private keys of a specific address.
TW_EXPORT_STRUCT
struct TWStarkExMessageSigner;

/// Sign a message.
///
/// \param privateKey: the private key used for signing
/// \param message: A custom hex message which is input to the signing.
/// \returns the signature, Hex-encoded. On invalid input empty string is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWStarkExMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message);

/// Verify signature for a message.
///
/// \param pubKey: pubKey that will verify and recover the message from the signature
/// \param message: the message signed (without prefix) in hex
/// \param signature: in Hex-encoded form.
/// \returns false on any invalid input (does not throw), true if the message can be recovered from the signature
TW_EXPORT_STATIC_METHOD
bool TWStarkExMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull pubKey, TWString* _Nonnull message, TWString* _Nonnull signature);

TW_EXTERN_C_END
