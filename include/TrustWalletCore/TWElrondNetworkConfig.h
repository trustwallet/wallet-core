// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWElrondNetworkConfig;

TW_EXPORT_STATIC_METHOD
struct TWElrondNetworkConfig *_Nonnull TWElrondNetworkConfigCreate();

TW_EXPORT_METHOD
void TWElrondNetworkConfigDelete(struct TWElrondNetworkConfig *_Nonnull self);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetChainId(struct TWElrondNetworkConfig *_Nonnull self, TWString *_Nonnull value);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetGasPerDataByte(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetMinGasLimit(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetMinGasPrice(struct TWElrondNetworkConfig *_Nonnull self, uint64_t value);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetGasCostESDTTransfer(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value);

TW_EXPORT_METHOD
void TWElrondNetworkConfigSetGasCostESDTNFTTransfer(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value);

TW_EXTERN_C_END
