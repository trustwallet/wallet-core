// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a signer to sign transactions for any blockchain.
struct TWAnySigner;

/// Signs a transaction specified by the signing input and coin type.
///
/// \param input The serialized data of a signing input (e.g. TW.Bitcoin.Proto.SigningInput).
/// \param coin The given coin type to sign the transaction for.
/// \return The serialized data of a `SigningOutput` proto object. (e.g. TW.Bitcoin.Proto.SigningOutput).
extern TWData *_Nonnull TWAnySignerSign(TWData *_Nonnull input, enum TWCoinType coin);

/// Signs a transaction specified by the JSON representation of signing input, coin type and a private key, returning the JSON representation of the signing output.
///
/// \param json JSON representation of a signing input
/// \param key The private key to sign with.
/// \param coin The given coin type to sign the transaction for.
/// \return The JSON representation of a `SigningOutput` proto object.
extern TWString *_Nonnull TWAnySignerSignJSON(TWString *_Nonnull json, TWData *_Nonnull key, enum TWCoinType coin);

/// Check if AnySigner supports signing JSON representation of signing input.
///
/// \param coin The given coin type to sign the transaction for.
/// \return true if AnySigner supports signing JSON representation of signing input for a given coin.
extern bool TWAnySignerSupportsJSON(enum TWCoinType coin);

/// Plans a transaction (for UTXO chains only).
///
/// \param input The serialized data of a signing input
/// \param coin The given coin type to plan the transaction for.
/// \return The serialized data of a `TransactionPlan` proto object.
extern TWData *_Nonnull TWAnySignerPlan(TWData *_Nonnull input, enum TWCoinType coin);

TW_EXTERN_C_END
