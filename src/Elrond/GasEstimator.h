// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include "NetworkConfig.h"

namespace TW::Elrond {

class GasEstimator {
    NetworkConfig networkConfig;
public:
    GasEstimator(const NetworkConfig& networkConfig);

    uint64_t forEGLDTransfer(size_t dataLength) const;
    uint64_t forESDTTransfer(size_t dataLength) const;
    uint64_t forESDTNFTTransfer(size_t dataLength) const;
};

} // namespace
