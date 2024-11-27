// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWPrivateKey.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a message signer to sign custom messages for any blockchain.
TW_EXPORT_CLASS
struct TWMessageSigner;

/// Signs an arbitrary message to prove ownership of an address for off-chain services.
///
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a `MessageSigningInput` proto object, (e.g. `TW.Solana.Proto.MessageSigningInput`).
/// \return The serialized data of a `MessageSigningOutput` proto object, (e.g. `TW.Solana.Proto.MessageSigningOutput`).
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWMessageSignerSign(enum TWCoinType coin, TWData* _Nonnull input);

/// Verifies a signature for a message.
///
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a verifying input (e.g. TW.Ethereum.Proto.MessageVerifyingInput).
/// \return whether the signature is valid.
TW_EXPORT_STATIC_METHOD
bool TWMessageSignerVerify(enum TWCoinType coin, TWData* _Nonnull input);

TW_EXTERN_C_END
