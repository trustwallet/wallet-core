// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NetworkConfig.h"

using namespace TW;
using namespace TW::Elrond;

NetworkConfig::NetworkConfig() :
    chainId("1"), // Mainnet
    gasPerDataByte(1500),
    minGasLimit(50000),
    minGasPrice(1000000000),
    gasCostESDTTransfer(200000),
    gasCostESDTNFTTransfer(200000) {
}

const std::string& NetworkConfig::getChainId() {
    return this->chainId;
}

void NetworkConfig::setChainId(const std::string& value) {
    this->chainId = value;
}

uint32_t NetworkConfig::getGasPerDataByte() {
    return this->gasPerDataByte;
}

void NetworkConfig::setGasPerDataByte(uint32_t value) {
    this->gasPerDataByte = value;
}

uint32_t NetworkConfig::getMinGasLimit() {
    return this->minGasLimit;
}

void NetworkConfig::setMinGasLimit(uint32_t value) {
    this->minGasLimit = value;
}

uint64_t NetworkConfig::getMinGasPrice() {
    return this->minGasPrice;
}

void NetworkConfig::setMinGasPrice(uint64_t value) {
    this->minGasPrice = value;
}

uint32_t NetworkConfig::getGasCostESDTTransfer() {
    return this->gasCostESDTTransfer;
}

void NetworkConfig::setGasCostESDTTransfer(uint32_t value) {
    this->gasCostESDTTransfer = value;
}

uint32_t NetworkConfig::getGasCostESDTNFTTransfer() {
    return this->gasCostESDTNFTTransfer;
}

void NetworkConfig::setGasCostESDTNFTTransfer(uint32_t value) {
    this->gasCostESDTNFTTransfer = value;
}
