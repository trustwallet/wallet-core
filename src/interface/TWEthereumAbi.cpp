// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbi.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>

#include "Data.h"
#include "Ethereum/ABI.h"
#include "Ethereum/ContractCall.h"
#include "HexCoding.h"
#include "uint256.h"

#include <cassert>
#include <string>
#include <vector>

using namespace TW;
namespace EthAbi = TW::Ethereum::ABI;

TWData* _Nonnull TWEthereumAbiDecodeContractCall(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& inputData = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(inputData);
    Rust::TWDataWrapper dataOutPtr = Rust::tw_ethereum_abi_decode_contract_call(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

TWData* _Nonnull TWEthereumAbiEncode(struct TWEthereumAbiFunction* _Nonnull func_in) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;

    Data encoded;
    function.encode(encoded);
    return TWDataCreateWithData(&encoded);
}

bool TWEthereumAbiDecodeOutput(struct TWEthereumAbiFunction* _Nonnull func_in,
                               TWData* _Nonnull encoded) {
    assert(func_in != nullptr);
    EthAbi::Function& function = func_in->impl;
    assert(encoded != nullptr);
    Data encData = data(TWDataBytes(encoded), TWDataSize(encoded));

    size_t offset = 0;
    return function.decodeOutput(encData, offset);
}

TWString* _Nullable TWEthereumAbiDecodeCall(TWData* _Nonnull callData, TWString* _Nonnull abiString) {
    const Data& call = *(reinterpret_cast<const Data*>(callData));
    const auto& jsonString = *reinterpret_cast<const std::string*>(abiString);
    try {
        auto string = EthAbi::decodeCall(call, jsonString);
        if (!string.has_value()) {
            return nullptr;
        }
        return TWStringCreateWithUTF8Bytes(string->c_str());
    }
    catch(...) {
        return nullptr;
    }
}

TWData* _Nonnull TWEthereumAbiEncodeTyped(TWString* _Nonnull messageJson) {
    Data data;
    try {
        data = EthAbi::ParamStruct::hashStructJson(TWStringUTF8Bytes(messageJson));
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(data.data(), data.size());
}
