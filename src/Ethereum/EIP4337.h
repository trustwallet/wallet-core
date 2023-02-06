// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "uint256.h"

namespace TW::Ethereum {

std::string getEIP4337DeploymentAddress(const std::string& factoryAddress, const std::string& logicAddress, const std::string& ownerAddress);

Data getEIP4337AccountInitializeBytecode(const std::string& ownerAddress, const std::string& factoryAddress);
Data getEIP4337LogicInitializeBytecode(const std::string& ownerAddress);
Data getEIP4337ExecuteBytecode(const Data& toAddress, const uint256_t& value, const Data& data);

}
