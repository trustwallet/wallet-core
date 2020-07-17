// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWData.h>

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWEthereumAbiValueDecoder;

/// Decodes input data (bytes longer than 32 will be truncated) as uint256
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumAbiValueDecoderDecodeUInt256(TWData* _Nonnull input);

TW_EXTERN_C_END
