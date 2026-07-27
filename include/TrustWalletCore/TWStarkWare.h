// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWPrivateKey.h"
#include "TWString.h"
#include "TWDerivationPath.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWStarkWare;

/// Generates the private stark key at the given derivation path from a valid eth signature
///
/// \param derivationPath non-null StarkEx Derivation path
/// \param signature valid eth signature
/// \return  The private key for the specified derivation path/signature, or `nullptr` if the signature or derivation path is invalid or an internal error occurs.
TW_EXPORT_STATIC_METHOD
struct TWPrivateKey* _Nullable TWStarkWareGetStarkKeyFromSignature(const struct TWDerivationPath* _Nonnull derivationPath, TWString* _Nonnull signature);

TW_EXTERN_C_END
