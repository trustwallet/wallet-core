// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "GasEstimator.h"

#include "../proto/Elrond.pb.h"

using namespace TW;
using namespace TW::Elrond;

// Additional gas to account for eventual increases in gas requirements (thus avoid breaking changes in clients of TW-core).
const uint64_t ADDITIONAL_GAS_FOR_ESDT_TRANSFER = 100000;

// Additional gas to account for extra blockchain operations (e.g. data movement (between accounts) for NFTs), 
// and for eventual increases in gas requirements (thus avoid breaking changes in clients of TW-core).
const uint64_t ADDITIONAL_GAS_FOR_ESDT_NFT_TRANSFER = 500000;

GasEstimator::GasEstimator(const NetworkConfig& networkConfig) {
    this->networkConfig = networkConfig;
}

uint64_t GasEstimator::forEGLDTransfer(size_t dataLength) const {
    uint64_t gasLimit = 
        this->networkConfig.getMinGasLimit() + 
        this->networkConfig.getGasPerDataByte() * dataLength;

    return gasLimit;
}

uint64_t GasEstimator::forESDTTransfer(size_t dataLength) const {
    uint64_t gasLimit = 
        this->networkConfig.getMinGasLimit() + 
        this->networkConfig.getGasCostESDTTransfer() + 
        this->networkConfig.getGasPerDataByte() * dataLength +
        ADDITIONAL_GAS_FOR_ESDT_TRANSFER;

    return gasLimit;
}

uint64_t GasEstimator::forESDTNFTTransfer(size_t dataLength) const {
    uint64_t gasLimit = 
        this->networkConfig.getMinGasLimit() + 
        this->networkConfig.getGasCostESDTNFTTransfer() + 
        this->networkConfig.getGasPerDataByte() * dataLength +
        ADDITIONAL_GAS_FOR_ESDT_NFT_TRANSFER;

    return gasLimit;
}
