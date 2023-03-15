// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
