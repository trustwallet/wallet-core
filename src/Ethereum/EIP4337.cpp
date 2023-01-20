// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EIP1014.h"
#include "EIP1967.h"
#include "AddressChecksum.h"
#include "Hash.h"
#include "HexCoding.h"
#include <iostream>
#include "ABI.h"

namespace TW::Ethereum {

Data getEIP4337LogicInitializeBytecode(const std::string& ownerAddress) {
    auto initializeFunc = ABI::Function("initialize", std::vector<std::shared_ptr<ABI::ParamBase>>{
                                                          std::make_shared<ABI::ParamAddress>(parse_hex(ownerAddress))
                                                      });
    Data initializeFuncEncoded;
    initializeFunc.encode(initializeFuncEncoded);
    return initializeFuncEncoded;
}

std::string getEIP4337DeploymentAddress(const std::string& factoryAddress, const std::string& logicAddress, const std::string& ownerAddress) {
    const Data logicInitializeBytecode = getEIP4337LogicInitializeBytecode(ownerAddress);
    const Data proxyInitCode = getEIP1967ProxyInitCode(logicAddress, logicInitializeBytecode);
    const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
    const Data initCodeHash = Hash::keccak256(proxyInitCode);
    return create2AddressString(factoryAddress, salt, initCodeHash);
}

} // namespace TW::Ethereum
