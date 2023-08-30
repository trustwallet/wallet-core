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

/// TODO...
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2Sign(enum TWCoinType coin, TWData* _Nonnull proto);

/// TODO...
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `PreSigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2PreImageHashes(enum TWCoinType coin, TWData* _Nonnull proto);

/// TODO...
///
/// \param proto the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
/// \return A pointer to the serialized `SigningOutput` protobuf structure from `BitcoinV2.proto`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinV2Compile(enum TWCoinType coin, TWData* _Nonnull proto);

TW_EXTERN_C_END
