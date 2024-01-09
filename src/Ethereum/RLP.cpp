// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "BinaryCoding.h"
#include "TrustWalletCore/TWCoinType.h"
#include "rust/Wrapper.h"

#include <tuple>

namespace TW::Ethereum {

Data RLP::encode(const EthereumRlp::Proto::EncodingInput& input) {
    Rust::TWDataWrapper inputData(data(input.SerializeAsString()));
    Rust::TWDataWrapper outputPtr = Rust::tw_ethereum_rlp_encode(TWCoinTypeEthereum, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return {};
    }

    EthereumRlp::Proto::EncodingOutput output;
    output.ParseFromArray(outputData.data(), static_cast<int>(outputData.size()));

    return data(output.encoded());
}

Data RLP::encodeString(const std::string& s) {
    EthereumRlp::Proto::EncodingInput input;
    input.mutable_item()->set_string_item(s);
    return encode(input);
}

Data RLP::encodeU256(const uint256_t & num) {
    auto numData = store(num);

    EthereumRlp::Proto::EncodingInput input;
    input.mutable_item()->set_number_u256(numData.data(), numData.size());
    return encode(input);
}

} // namespace TW::Ethereum
