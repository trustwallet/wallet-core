// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWPrivateKey.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

/// Tezos message signing, verification and utilities.
TW_EXPORT_STRUCT
struct TWTezosMessageSigner;

/// Implement format input as described in https://tezostaquito.io/docs/signing/
///
/// \param message message to format e.g: Hello, World
/// \param dAppUrl the app url, e.g: testUrl
/// \returns the formatted message as a string
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWTezosMessageSignerFormatMessage(TWString* _Nonnull message, TWString* _Nonnull url);

/// Implement input to payload as described in: https://tezostaquito.io/docs/signing/
///
/// \param message formatted message to be turned into an hex payload
/// \return the hexpayload of the formated message as a hex string
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWTezosMessageSignerInputToPayload(TWString* _Nonnull message);

/// Sign a message as described in https://tezostaquito.io/docs/signing/
///
/// \param privateKey: the private key used for signing
/// \param message: A custom message payload (hex) which is input to the signing.
/// \returns the signature, Hex-encoded. On invalid input empty string is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWTezosMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull message);

/// Verify signature for a message as described in https://tezostaquito.io/docs/signing/
///
/// \param pubKey: pubKey that will verify the message from the signature
/// \param message: the message signed as a payload (hex)
/// \param signature: in Base58-encoded form.
/// \returns false on any invalid input (does not throw), true if the message can be verified from the signature
TW_EXPORT_STATIC_METHOD
bool TWTezosMessageSignerVerifyMessage(const struct TWPublicKey* _Nonnull pubKey, TWString* _Nonnull message, TWString* _Nonnull signature);

TW_EXTERN_C_END
