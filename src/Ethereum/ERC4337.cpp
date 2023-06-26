// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ABI.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "EIP1967.h"
#include "Hash.h"
#include "HexCoding.h"
#include <iostream>

namespace TW::Ethereum {

using ParamBasePtr = std::shared_ptr<ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;

// https://github.com/eth-infinitism/account-abstraction/blob/develop/contracts/samples/SimpleAccount.sol#L57
Data getERC4337ExecuteBytecode(const Data& toAddress, const uint256_t& value, const Data& data) {
    auto executeFunc = ABI::Function("execute", ParamCollection{
                                                    std::make_shared<ABI::ParamAddress>(toAddress),
                                                    std::make_shared<ABI::ParamUInt256>(value),
                                                    std::make_shared<ABI::ParamByteArray>(data)});
    Data executeFuncEncoded;
    executeFunc.encode(executeFuncEncoded);
    return executeFuncEncoded;
}

// https://github.com/eth-infinitism/account-abstraction/blob/develop/contracts/samples/SimpleAccount.sol#L65
Data getERC4337ExecuteBatchBytecode(const std::vector<Data>& toAddresses, const std::vector<uint256_t>& amounts, const std::vector<Data>& payloads) {
    auto addressesParam = ABI::ParamArray();
    for (const auto& toAddress: toAddresses) {
        addressesParam.addParam(std::make_shared<ABI::ParamAddress>(toAddress));
    }
    auto valuesParam = ABI::ParamArray();
    for (const auto& amount: amounts) {
        valuesParam.addParam(std::make_shared<ABI::ParamUInt256>(amount));
    }
    auto payloadsParam = ABI::ParamArray();
    for (const auto& payload: payloads) {
        payloadsParam.addParam(std::make_shared<ABI::ParamByteArray>(payload));
    }
    auto executeFunc = ABI::Function("executeBatch", ParamCollection{
                                                            std::make_shared<ABI::ParamArray>(addressesParam),
                                                            std::make_shared<ABI::ParamArray>(valuesParam),
                                                            std::make_shared<ABI::ParamArray>(payloadsParam)});
    Data executeFuncEncoded;
    executeFunc.encode(executeFuncEncoded);
    return executeFuncEncoded;
}


} // namespace TW::Ethereum
