// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereum;

/// Generate a layer 2 eip2645 derivation path from eth address, layer, application and given index.
///
/// \param wallet non-null TWHDWallet
/// \param ethAddress non-null Ethereum address
/// \param layer  non-null layer 2 name (E.G starkex)
/// \param application non-null layer 2 application (E.G immutablex)
/// \param index non-null layer 2 index (E.G 1)
/// \return a valid eip2645 layer 2 derivation path as a string
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumEip2645GetPath(TWString* _Nonnull ethAddress, TWString* _Nonnull layer,  TWString* _Nonnull application,  TWString* _Nonnull index);

TW_EXTERN_C_END
