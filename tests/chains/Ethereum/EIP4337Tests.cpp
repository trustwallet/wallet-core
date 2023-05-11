// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWEthereum.h>
#include <TrustWalletCore/TWHash.h>
#include <Ethereum/EIP4337.h>
#include <HexCoding.h>
#include <PrivateKey.h>

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {

TEST(EthereumEip4337, GetEIP4337AccountInitializeBytecode) {
    {
        const std::string& ownerAddress = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
        const std::string& factoryAddress = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
        const auto& initializeEncoded = Ethereum::getEIP4337AccountInitializeBytecode(ownerAddress, factoryAddress);
        ASSERT_EQ(hexEncoded(initializeEncoded), "0x5a87209b755781cf65feeedd3855ade0317f4a925fbfb9cf00000000000000000000000078d9c32b96bb872d66d51818227563f44e67e2380000000000000000000000000000000000000000000000000000000000000000");
    }
}

TEST(EthereumEip4337, GetEIP4337LogicInitializeBytecode) {
    {
        const std::string& ownerAddress = "0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988";
        const auto& initializeEncoded = Ethereum::getEIP4337LogicInitializeBytecode(ownerAddress);
        ASSERT_EQ(hexEncoded(initializeEncoded), "0xc4d66de8000000000000000000000000a5a1dddef094095afb7b6e322de72961df2e1988");
    }
}

TEST(EthereumEip4337, GetEIP4337ExecuteBytecode) {
    {
        const Data& toAddress = parse_hex("0xce642355Fa553f408C34a2650Ad2F4A1634d033a");
        const uint256_t& value = 0x2386f26fc10000;
        const Data data = {};
        const auto& executeEncoded = Ethereum::getEIP4337ExecuteBytecode(toAddress, value, data);
        ASSERT_EQ(hexEncoded(executeEncoded), "0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000");
    }
}

//
// https://goerli.etherscan.io/address/0x5a87209b755781cf65feeedd3855ade0317f4a92#readContract
TEST(EthereumEip4337, GetEIP4337DeploymentAddress) {
    // C++
    {
        const std::string& factoryAddress = "0x5A87209b755781cF65fEeEdd3855ade0317f4a92";
        const std::string& logicAddress = "0x21cc27d7db4fa19857a3702653a7a67ee30ca620";
        const std::string& ownerAddress = "0x78d9C32b96Bb872D66D51818227563f44e67E238";
        const std::string& deploymentAddress = Ethereum::getEIP4337DeploymentAddress(factoryAddress, logicAddress, ownerAddress);
        ASSERT_EQ(deploymentAddress, "0x8cE23B8769ac01d0df0d5f47Be1A38FeA97F3879");
    }

    // C
    {
        const auto factoryAddress = STRING("0x5A87209b755781cF65fEeEdd3855ade0317f4a92");
        const auto logicAddress = STRING("0x21cc27d7db4fa19857a3702653a7a67ee30ca620");
        const auto ownerAddress = STRING("0x78d9C32b96Bb872D66D51818227563f44e67E238");
        const auto& result = WRAPS(TWEthereumEip4337GetDeploymentAddress(factoryAddress.get(), logicAddress.get(), ownerAddress.get()));
        assertStringsEqual(result, "0x8cE23B8769ac01d0df0d5f47Be1A38FeA97F3879");
    }
}

}
