// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Ethereum {

std::string getEIP4337DeploymentAddress(const std::string& factoryAddress, const std::string& logicAddress, const std::string& ownerAddress);

// https://github.com/thomas-waite/bundler/blob/b083680059a52d3121c5e33cea67c86652370562/packages/sdk/src/SimpleAccountAPI.ts#L63-L75
Data getEIP4337AccountInitializeBytecode(const std::string& ownerAddress, const std::string& factoryAddress);

// https://github.com/eth-infinitism/account-abstraction/blob/5a1ad4072438d9e9f7c934b66464dc05a4b37d02/contracts/samples/SimpleAccountFactory.sol#L48
Data getEIP4337LogicInitializeBytecode(const std::string& ownerAddress);

}
