// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Filecoin-Ethereum address converter.
TW_EXPORT_STRUCT
struct TWFilecoinAddressConverter;

/// Converts a Filecoin address to Ethereum.
///
/// \param filecoinAddress: a Filecoin address.
/// \returns the Ethereum address. On invalid input empty string is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWFilecoinAddressConverterConvertToEthereum(TWString* _Nonnull filecoinAddress);

/// Converts an Ethereum address to Filecoin.
///
/// \param ethAddress: an Ethereum address.
/// \returns the Filecoin address. On invalid input empty string is returned. Returned object needs to be deleted after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWFilecoinAddressConverterConvertFromEthereum(TWString* _Nonnull ethAddress);

TW_EXTERN_C_END
