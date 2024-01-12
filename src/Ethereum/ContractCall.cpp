// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "ContractCall.h"
#include "HexCoding.h"
#include "proto/EthereumAbi.pb.h"
#include "TrustWalletCore/TWCoinType.h"

using namespace std;
using json = nlohmann::json;

namespace TW::Ethereum::ABI {

optional<string> decodeCall(const Data& call, const std::string& abi) {
    EthereumAbi::Proto::ContractCallDecodingInput input;
    input.set_encoded(call.data(), call.size());
    input.set_smart_contract_abi_json(abi);

    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_abi_decode_contract_call(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    EthereumAbi::Proto::ContractCallDecodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    if (output.error() != EthereumAbi::Proto::AbiError::OK) {
        return {};
    }

    return output.decoded_json();
}

} // namespace TW::Ethereum::ABI
