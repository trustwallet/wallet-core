// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NetworkConfig.h"

#include <chrono>

using namespace TW;
using namespace TW::Elrond;
using namespace std::chrono;

NetworkConfig::NetworkConfig() :
    chainId("1") /* Mainnet */ {
}

const std::string& NetworkConfig::getChainId() const {
    return this->chainId;
}

void NetworkConfig::setChainId(const std::string& value) {
    this->chainId = value;
}

uint32_t NetworkConfig::getGasPerDataByte() const {
    return this->gasPerDataByte;
}

void NetworkConfig::setGasPerDataByte(uint32_t value) {
    this->gasPerDataByte = value;
}

uint32_t NetworkConfig::getMinGasLimit() const {
    return this->minGasLimit;
}

void NetworkConfig::setMinGasLimit(uint32_t value) {
    this->minGasLimit = value;
}

uint64_t NetworkConfig::getMinGasPrice() const {
    return this->minGasPrice;
}

void NetworkConfig::setMinGasPrice(uint64_t value) {
    this->minGasPrice = value;
}

uint32_t NetworkConfig::getGasCostESDTTransfer() const {
    return this->gasCostESDTTransfer;
}

void NetworkConfig::setGasCostESDTTransfer(uint32_t value) {
    this->gasCostESDTTransfer = value;
}

uint32_t NetworkConfig::getGasCostESDTNFTTransfer() const {
    return this->gasCostESDTNFTTransfer;
}

void NetworkConfig::setGasCostESDTNFTTransfer(uint32_t value) {
    this->gasCostESDTNFTTransfer = value;
}

NetworkConfig NetworkConfig::GetDefault() {
    const uint64_t timestamp = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    return GetByTimestamp(timestamp);
}

NetworkConfig NetworkConfig::GetByTimestamp(uint64_t timestamp) {
    NetworkConfig networkConfig;

    // Mainnet values at the time of defining the "NetworkConfig" component (December 2021).
    if (timestamp > 0) {
        networkConfig.setGasPerDataByte(1500);
        networkConfig.setMinGasLimit(50000);
        networkConfig.setMinGasPrice(1000000000);
        networkConfig.setGasCostESDTTransfer(200000);
        networkConfig.setGasCostESDTNFTTransfer(200000);
    }

    return networkConfig;
}
