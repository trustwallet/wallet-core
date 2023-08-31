// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Bitcoin script manipulating functions
TW_EXPORT_CLASS
struct TWBitcoinV2;

/// Builds and signs a Bitcoin transaction. This function essentially combines the
/// `TWBitcoinV2PreImageHashes` and `TWBitcoinV2Compile` functionality.
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2Sign(enum TWCoinType coin, TWData* _Nonnull proto);

/// Generates the sighashes for each input that must be signed. This function
/// can be used if the private key(s) cannot be directly exposed to this
/// library. The generated signatures can then be passed on to
/// `TWBitcoinV2Compile` in order to build the final Bitcoin transaction.
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `PreSigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2PreImageHashes(enum TWCoinType coin, TWData* _Nonnull proto);

/// Builds a Bitcoin transaction from the given signatures. This function is
/// normally called after signing the sighashes returned from `TWBitcoinV2PreImageHashes`.
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2Compile(enum TWCoinType coin, TWData* _Nonnull proto);

TW_EXTERN_C_END
