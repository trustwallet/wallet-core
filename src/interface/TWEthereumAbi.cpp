// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWEthereumAbi.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>

#include "Data.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ContractCall.h"
#include "Ethereum/MessageSigner.h"
#include "HexCoding.h"

#include <cassert>
#include <vector>

using namespace TW;
namespace EthAbi = TW::Ethereum::ABI;

template <typename F>
static TWData* _Nonnull ethereumAbiForwardToRust(F rustFunction, enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& inputData = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(inputData);
    Rust::TWDataWrapper dataOutPtr = rustFunction(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

TWData* _Nonnull TWEthereumAbiDecodeContractCall(enum TWCoinType coin, TWData* _Nonnull input) {
    return ethereumAbiForwardToRust(Rust::tw_ethereum_abi_decode_contract_call, coin, input);
}

TWData* _Nonnull TWEthereumAbiDecodeParams(enum TWCoinType coin, TWData* _Nonnull input) {
    return ethereumAbiForwardToRust(Rust::tw_ethereum_abi_decode_params, coin, input);
}

TWData* _Nonnull TWEthereumAbiDecodeValue(enum TWCoinType coin, TWData* _Nonnull input) {
    return ethereumAbiForwardToRust(Rust::tw_ethereum_abi_decode_value, coin, input);
}

TWData* _Nonnull TWEthereumAbiEncodeFunction(enum TWCoinType coin, TWData* _Nonnull input) {
    return ethereumAbiForwardToRust(Rust::tw_ethereum_abi_encode_function, coin, input);
}

TWData* _Nonnull TWEthereumAbiEncode(struct TWEthereumAbiFunction* _Nonnull func_in) {
    assert(func_in != nullptr);
    Data encodedData;
    auto encoded = func_in->impl.encodeInput();
    if (encoded.has_value()) {
        encodedData = encoded.value();
    }
    return TWDataCreateWithData(&encodedData);
}

bool TWEthereumAbiDecodeOutput(struct TWEthereumAbiFunction* _Nonnull func_in,
                               TWData* _Nonnull encoded) {
    assert(func_in != nullptr);
    assert(encoded != nullptr);
    const Data& encData = *(reinterpret_cast<const Data*>(encoded));

    bool isOutput = true;
    return func_in->impl.decode(encData, isOutput);
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
        data = Ethereum::MessageSigner::typedDataPreImageHash(TWStringUTF8Bytes(messageJson));
    } catch (...) {} // return empty
    return TWDataCreateWithBytes(data.data(), data.size());
}
