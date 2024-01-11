// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ValueDecoder.h"
#include "proto/EthereumAbi.pb.h"
#include "rust/Wrapper.h"
#include "TrustWalletCore/TWCoinType.h"

namespace TW::Ethereum::ABI {

uint256_t ValueDecoder::decodeUInt256(const Data& data) {
    if (data.size() > 32) {
        return load(subData(data, 0, 32));
    }
    return load(data);
}

std::string ValueDecoder::decodeValue(const Data& encoded, const std::string& type) {
    EthereumAbi::Proto::ValueDecodingInput input;
    input.set_encoded(encoded.data(), encoded.size());
    input.set_param_type(type);

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_abi_decode_value(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return "";
    }

    EthereumAbi::Proto::ValueDecodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != EthereumAbi::Proto::AbiError::OK) {
        return "";
    }

    return output.param_str();
}

} // namespace TW::Ethereum::ABI
