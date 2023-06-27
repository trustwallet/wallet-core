// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::MultiversX {

/// A "TransactionFactoryConfig" object holds the network parameters relevant to creating transactions (e.g. minimum gas limit, minimum gas price).
class TransactionFactoryConfig {
    /// The following fields can (should) be fetched from https://api.multiversx.com/network/config.
    /// However, a "TransactionFactoryConfig" object is initialized with proper default values for Mainnet (as of 2023).
    std::string chainId;
    uint32_t gasPerDataByte;
    uint32_t minGasLimit;
    uint32_t extraGasLimitForGuardedTransaction;
    uint64_t minGasPrice;

    /// GasSchedule entries of interest (only one at this moment), according to: https://github.com/multiversx/mx-chain-mainnet-config/blob/master/gasSchedules.
    /// Here, for the sake of simplicity, we define the gas costs of interest directly in the class "TransactionFactoryConfig"
    /// (that is, without defining extra nested structures such as "GasSchedule" and "BuiltInCosts").
    uint32_t gasCostESDTTransfer;
    uint32_t gasCostESDTNFTTransfer;

    // Additional gas to account for eventual increases in gas requirements (thus avoid breaking changes in clients of TW-core).
    uint64_t additionalGasForESDTTransfer;

    // Additional gas to account for extra blockchain operations (e.g. data movement (between accounts) for NFTs),
    // and for eventual increases in gas requirements (thus avoid breaking changes in clients of TW-core).
    uint64_t additionalGasForESDTNFTTransfer;

public:
    TransactionFactoryConfig();

    const std::string& getChainId() const;
    void setChainId(const std::string& value);

    uint32_t getGasPerDataByte() const;
    void setGasPerDataByte(uint32_t value);

    uint32_t getMinGasLimit() const;
    void setMinGasLimit(uint32_t value);

    uint32_t getExtraGasLimitForGuardedTransaction() const;
    void setExtraGasLimitForGuardedTransaction(uint32_t value);

    uint64_t getMinGasPrice() const;
    void setMinGasPrice(uint64_t value);

    uint32_t getGasCostESDTTransfer() const;
    void setGasCostESDTTransfer(uint32_t value);

    uint64_t getAdditionalGasForESDTTransfer() const;
    void setAdditionalGasForESDTTransfer(uint64_t value);

    uint64_t getAdditionalGasForESDTNFTTransfer() const;
    void setAdditionalGasForESDTNFTTransfer(uint64_t value);

    uint32_t getGasCostESDTNFTTransfer() const;
    void setGasCostESDTNFTTransfer(uint32_t value);

    static TransactionFactoryConfig GetDefault();

    /// Useful to implement upwards-compatible changes of the network configuration (a TWCore client can receive planned configuration updates, in advance).
    static TransactionFactoryConfig GetByTimestamp(uint64_t timestamp);
};

} // namespace TW::MultiversX
