// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWMessageSigner;

/// Computes preimage hashes of a message.
/// 
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a signing input (e.g. TW.Ethereum.Proto.MessageSigningInput).
/// \return The serialized data of TW.TxCompiler.PreSigningOutput.
TW_EXPORT_STATIC_METHOD TWData *_Nullable TWMessageSignerPreImageHashes(enum TWCoinType coin, TWData *_Nonnull input);

/// Verifies a signature for a message.
/// 
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a verifying input (e.g. TW.Ethereum.Proto.MessageVerifyingInput).
/// \return whether the signature is valid.
TW_EXPORT_STATIC_METHOD bool TWMessageSignerVerify(enum TWCoinType coin, TWData *_Nonnull input);

/// Signs an arbitrary message to prove ownership of an address for off-chain services.
/// 
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a signing input (e.g. TW.Ethereum.Proto.MessageSigningInput).
/// \return The serialized data of a `SigningOutput` proto object. (e.g. TW.Ethereum.Proto.MessageSigningOutput).
TW_EXPORT_STATIC_METHOD TWData *_Nullable TWMessageSignerSign(enum TWCoinType coin, TWData *_Nonnull input);

TW_EXTERN_C_END
