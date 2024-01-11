// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "CoinEntry.h"

#include <google/protobuf/util/json_util.h>

namespace TW::Rust {

/// The function takes a Protobuf output message `const Output&` and returns `std::string`.
template<typename Output>
using SignJSONOutputHandler = std::function<std::string(const Output&)>;

class RustCoinEntry : public CoinEntry {
public:
    ~RustCoinEntry() noexcept override = default;
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const override;
    std::string normalizeAddress(TWCoinType coin, const std::string& address) const override;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const override;
    Data addressToData(TWCoinType coin, const std::string& address) const override;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;

    Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

class RustCoinEntryWithSignJSON: public RustCoinEntry {
public:
    ~RustCoinEntryWithSignJSON() noexcept override = default;

protected:
    /// Helper function that can be used by [`Entry::signJSON`] to:
    /// 1. Deserialize the given `json` as an `Input` object.
    /// 2. Put the given `key` as `Input::private_key`.
    /// 3. Serialize the input as bytes and call `Entry::sign`.
    /// 4. Deserialize the output bytes as an `Output` object.
    /// 5. Map the output object to a string.
    template <typename Input, typename Output>
    std::string signJSONHelper(TWCoinType coin, const std::string &json, const Data &key,
                               SignJSONOutputHandler<Output> mapOutput) const {
        Input input;
        google::protobuf::util::JsonStringToMessage(json, &input);
        input.set_private_key(key.data(), key.size());

        auto inputData = data(input.SerializeAsString());
        Data dataOut;
        sign(coin, inputData, dataOut);

        if (dataOut.empty()) {
            return {};
        }

        Output output;
        output.ParseFromArray(dataOut.data(), static_cast<int>(dataOut.size()));

        return mapOutput(output);
    }
};

} // namespace TW::Rust
