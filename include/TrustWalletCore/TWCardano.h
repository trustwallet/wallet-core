// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Cardano helper functions
TW_EXPORT_STRUCT
struct TWCardano;

/// Calculates the minimum ADA amount needed for a UTXO.
///
/// \deprecated consider using `TWCardanoOutputMinAdaAmount` instead.
/// \see reference https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
/// \param tokenBundle serialized data of TW.Cardano.Proto.TokenBundle.
/// \return the minimum ADA amount.
TW_EXPORT_STATIC_METHOD
uint64_t TWCardanoMinAdaAmount(TWData *_Nonnull tokenBundle) TW_VISIBILITY_DEFAULT;

/// Calculates the minimum ADA amount needed for an output.
///
/// \see reference https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
/// \param toAddress valid destination address, as string.
/// \param tokenBundle serialized data of TW.Cardano.Proto.TokenBundle.
/// \param coinsPerUtxoByte cost per one byte of a serialized UTXO (Base-10 decimal string).
/// \return the minimum ADA amount (Base-10 decimal string).
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWCardanoOutputMinAdaAmount(TWString *_Nonnull toAddress, TWData *_Nonnull tokenBundle, TWString *_Nonnull coinsPerUtxoByte) TW_VISIBILITY_DEFAULT;

/// Return the staking address associated to (contained in) this address. Must be a Base address.
/// Empty string is returned on error. Result must be freed.
/// \param baseAddress A valid base address, as string.
/// \return the associated staking (reward) address, as string, or empty string on error.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCardanoGetStakingAddress(TWString *_Nonnull baseAddress) TW_VISIBILITY_DEFAULT;

/// Return the legacy(byron) address.
/// \param publicKey A valid public key with TWPublicKeyTypeED25519Cardano type.
/// \return the legacy(byron) address, as string, or empty string on error.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWCardanoGetByronAddress(struct TWPublicKey *_Nonnull publicKey);

TW_EXTERN_C_END
