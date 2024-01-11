// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"

namespace TW::EOS {

enum class CompressionType {
    None = 0,
};

NLOHMANN_JSON_SERIALIZE_ENUM( CompressionType, {
{CompressionType::None, "none"},
})

class PackedTransaction {
public:
    std::vector<Signature> signatures;
    CompressionType compression;
    Data packedCFD;
    Data packedTrx;

    PackedTransaction(const Transaction& transaction, CompressionType type = CompressionType::None) noexcept;

    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const noexcept;
};

} // namespace TW::EOS
