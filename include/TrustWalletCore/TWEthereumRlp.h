// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereumRlp;

/// Encode an item or a list of items as Eth RLP binary format.
///
/// \param coin EVM-compatible coin type.
/// \param input Non-null serialized `EthereumRlp::Proto::EncodingInput`.
/// \return serialized `EthereumRlp::Proto::EncodingOutput`.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWEthereumRlpEncode(enum TWCoinType coin, TWData* _Nonnull input);

TW_EXTERN_C_END
