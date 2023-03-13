// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWPrivateKey.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

/// Tron message signing and verification.
///
/// Tron and some other wallets support a message signing & verification format, to create a proof (a signature)
/// that someone has access to the private keys of a specific address.
TW_EXPORT_STRUCT
struct TWTronMessageSigner;

/// Sign a message.
///
/// \param privateKey: the private key used for signing
/// \param message: A custom message which is input to the signing.
/// \returns the signature, Hex-encoded. On invalid input empty string is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWTronMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message);

/// Verify signature for a message.
///
/// \param pubKey: pubKey that will verify and recover the message from the signature
/// \param message: the message signed (without prefix)
/// \param signature: in Hex-encoded form.
/// \returns false on any invalid input (does not throw), true if the message can be recovered from the signature
TW_EXPORT_STATIC_METHOD
bool TWTronMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull pubKey, TWString* _Nonnull message, TWString* _Nonnull signature);

TW_EXTERN_C_END
