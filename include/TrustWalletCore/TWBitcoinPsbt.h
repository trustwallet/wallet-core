// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWBitcoinSigHashType.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

/// Represents a signer to sign/plan PSBT for Bitcoin blockchains.
TW_EXPORT_CLASS
struct TWBitcoinPsbt;

/// Signs a PSBT (Partially Signed Bitcoin Transaction) specified by the signing input and coin type.
///
/// \param input The serialized data of a signing input (e.g. `TW.BitcoinV2.Proto.PsbtSigningInput`)
/// \param coin The given coin type to sign the PSBT for.
/// \return The serialized data of a `Proto.PsbtSigningOutput` proto object (e.g. `TW.BitcoinV2.Proto.PsbtSigningOutput`).
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinPsbtSign(TWData* _Nonnull input, enum TWCoinType coin);

/// Plans a PSBT (Partially Signed Bitcoin Transaction).
/// Can be used to get the transaction detailed decoded from PSBT.
///
/// \param input The serialized data of a signing input (e.g. `TW.BitcoinV2.Proto.PsbtSigningInput`)
/// \param coin The given coin type to sign the PSBT for.
/// \return The serialized data of a `Proto.TransactionPlan` proto object (e.g. `TW.BitcoinV2.Proto.TransactionPlan`).
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinPsbtPlan(TWData* _Nonnull input, enum TWCoinType coin);

TW_EXTERN_C_END
