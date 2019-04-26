#pragma once

#include "Transaction.h"

namespace TW::EOS {
enum class CompressionType {
    None = 0,
    // ZLIB = 1,
};

NLOHMANN_JSON_SERIALIZE_ENUM( CompressionType, {
{CompressionType::None, "none"},
// // {CompressionType::ZLIB, "zlib"},
})

class PackedTransaction {
public:
    std::vector<Signature> signatures;
    CompressionType compression;
    Data packedCFD;
    Data packedTrx;

    PackedTransaction(const Transaction& transaction, CompressionType type) noexcept;

    void serialize(Data& os) const noexcept;
    nlohmann::json serialize() const noexcept;
};
} // namespace TW::EOS