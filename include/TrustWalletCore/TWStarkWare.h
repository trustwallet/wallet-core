// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
/// \return  The private key for the specified derivation path/signature
TW_EXPORT_STATIC_METHOD
struct TWPrivateKey* _Nonnull TWStarkWareGetStarkKeyFromSignature(const struct TWDerivationPath* _Nonnull derivationPath, TWString* _Nonnull signature);

TW_EXTERN_C_END
