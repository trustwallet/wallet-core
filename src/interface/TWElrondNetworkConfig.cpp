// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWElrondNetworkConfig.h>

#include "Elrond/NetworkConfig.h"
#include "Data.h"
#include "assert.h"

using namespace TW;
using namespace TW::Elrond;

struct TWElrondNetworkConfig *_Nonnull TWElrondNetworkConfigCreate() {
    auto networkConfig = NetworkConfig();
    return new TWElrondNetworkConfig{ networkConfig };
}

void TWElrondNetworkConfigDelete(struct TWElrondNetworkConfig *_Nonnull self) {
    assert(self != nullptr);
    delete self;
}

void TWElrondNetworkConfigSetChainId(struct TWElrondNetworkConfig *_Nonnull self, TWString *_Nonnull value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setChainId(TWStringUTF8Bytes(value));
}

void TWElrondNetworkConfigSetGasPerDataByte(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setGasPerDataByte(value);
}

void TWElrondNetworkConfigSetMinGasLimit(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setMinGasLimit(value);
}

void TWElrondNetworkConfigSetMinGasPrice(struct TWElrondNetworkConfig *_Nonnull self, uint64_t value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setMinGasPrice(value);
}

void TWElrondNetworkConfigSetGasCostESDTTransfer(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setGasCostESDTTransfer(value);
}

void TWElrondNetworkConfigSetGasCostESDTNFTTransfer(struct TWElrondNetworkConfig *_Nonnull self, uint32_t value) {
    NetworkConfig& networkConfig = self->impl;
    networkConfig.setGasCostESDTNFTTransfer(value);
}
