// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionFactoryConfig.h"

#include <chrono>

namespace TW::MultiversX {

TransactionFactoryConfig::TransactionFactoryConfig()
    : chainId("1") /* Mainnet */ {
}

const std::string& TransactionFactoryConfig::getChainId() const {
    return this->chainId;
}

void TransactionFactoryConfig::setChainId(const std::string& value) {
    this->chainId = value;
}

uint32_t TransactionFactoryConfig::getGasPerDataByte() const {
    return this->gasPerDataByte;
}

void TransactionFactoryConfig::setGasPerDataByte(uint32_t value) {
    this->gasPerDataByte = value;
}

uint32_t TransactionFactoryConfig::getMinGasLimit() const {
    return this->minGasLimit;
}

void TransactionFactoryConfig::setMinGasLimit(uint32_t value) {
    this->minGasLimit = value;
}

uint32_t TransactionFactoryConfig::getExtraGasLimitForGuardedTransaction() const {
    return this->minGasLimit;
}

void TransactionFactoryConfig::setExtraGasLimitForGuardedTransaction(uint32_t value) {
    this->extraGasLimitForGuardedTransaction = value;
}

uint64_t TransactionFactoryConfig::getMinGasPrice() const {
    return this->minGasPrice;
}

void TransactionFactoryConfig::setMinGasPrice(uint64_t value) {
    this->minGasPrice = value;
}

uint32_t TransactionFactoryConfig::getGasCostESDTTransfer() const {
    return this->gasCostESDTTransfer;
}

void TransactionFactoryConfig::setGasCostESDTTransfer(uint32_t value) {
    this->gasCostESDTTransfer = value;
}

uint32_t TransactionFactoryConfig::getGasCostESDTNFTTransfer() const {
    return this->gasCostESDTNFTTransfer;
}

void TransactionFactoryConfig::setGasCostESDTNFTTransfer(uint32_t value) {
    this->gasCostESDTNFTTransfer = value;
}

uint64_t TransactionFactoryConfig::getAdditionalGasForESDTTransfer() const {
    return this->additionalGasForESDTTransfer;
}

void TransactionFactoryConfig::setAdditionalGasForESDTTransfer(uint64_t value) {
    this->additionalGasForESDTTransfer = value;
}

uint64_t TransactionFactoryConfig::getAdditionalGasForESDTNFTTransfer() const {
    return this->additionalGasForESDTNFTTransfer;
}

void TransactionFactoryConfig::setAdditionalGasForESDTNFTTransfer(uint64_t value) {
    this->additionalGasForESDTNFTTransfer = value;
}

TransactionFactoryConfig TransactionFactoryConfig::GetDefault() {
    const uint64_t timestamp = duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return GetByTimestamp(timestamp);
}

TransactionFactoryConfig TransactionFactoryConfig::GetByTimestamp(uint64_t timestamp) {
    TransactionFactoryConfig config;

    // Mainnet values at the time of defining the "TransactionFactoryConfig" component (April / May 2023).
    if (timestamp > 0) {
        config.setGasPerDataByte(1500);
        config.setMinGasLimit(50000);
        config.setExtraGasLimitForGuardedTransaction(50000);
        config.setMinGasPrice(1000000000);
        config.setGasCostESDTTransfer(200000);
        config.setGasCostESDTNFTTransfer(200000);
        config.setAdditionalGasForESDTTransfer(100000);
        config.setAdditionalGasForESDTNFTTransfer(500000);
    }

    return config;
}

} // namespace TW::MultiversX
