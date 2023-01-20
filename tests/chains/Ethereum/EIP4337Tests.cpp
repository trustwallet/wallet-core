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

TEST(EthereumEip4337, GetEIP4337LogicInitializeBytecode) {
    {
        const std::string& ownerAddress = "0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988";
        const auto& initializeEncoded = Ethereum::getEIP4337LogicInitializeBytecode(ownerAddress);
        ASSERT_EQ(hexEncoded(initializeEncoded), "0xc4d66de8000000000000000000000000a5a1dddef094095afb7b6e322de72961df2e1988");
    }
}

TEST(EthereumEip4337, GetEIP4337DeploymentAddress) {
    // C++
    {
        const std::string& factoryAddress = "0xd9145CCE52D386f254917e481eB44e9943F39138";
        const std::string& logicAddress = "0x5C9eb5D6a6C2c1B3EFc52255C0b356f116f6f66D";
        const std::string& ownerAddress = "0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988";
        const std::string& deploymentAddress = Ethereum::getEIP4337DeploymentAddress(factoryAddress, logicAddress, ownerAddress);
        ASSERT_EQ(deploymentAddress, "0xbEaA87cEEaC906C21aaacd258FbFB87CfA3c90a8");
    }

    // C
    {
        const auto factoryAddress = STRING("0xd9145CCE52D386f254917e481eB44e9943F39138");
        const auto logicAddress = STRING("0x5C9eb5D6a6C2c1B3EFc52255C0b356f116f6f66D");
        const auto ownerAddress = STRING("0xA5a1dddEF094095AfB7b6e322dE72961DF2e1988");
        const auto& result = WRAPS(TWEthereumEip4337GetDeploymentAddress(factoryAddress.get(), logicAddress.get(), ownerAddress.get()));
        assertStringsEqual(result, "0xbEaA87cEEaC906C21aaacd258FbFB87CfA3c90a8");
    }
}

}
