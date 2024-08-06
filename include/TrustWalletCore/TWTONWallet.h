// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWPublicKey.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// TON wallet operations.
TW_EXPORT_CLASS
struct TWTONWallet;

/// Constructs a TON Wallet V4R2 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
///
/// \param publicKey wallet's public key.
/// \param workchain TON workchain to which the wallet belongs. Usually, base chain is used (0).
/// \param walletId wallet's ID allows to create multiple wallets for the same private key.
/// \return Pointer to a base64 encoded Bag Of Cells (BoC) StateInit. Null if invalid public key provided.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONWalletBuildV4R2StateInit(struct TWPublicKey *_Nonnull publicKey, int32_t workchain, int32_t walletId);

TW_EXTERN_C_END
