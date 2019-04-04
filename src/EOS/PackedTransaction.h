#pragma once

#include "Transaction.h"

namespace TW {
namespace EOS {
    enum class CompressionType {
        None = 0,
        // ZLIB = 1,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM( CompressionType, {
    {CompressionType::None, "none"},
    // // {CompressionType::ZLIB, "zlib"},
    })

    class PackedTransaction: Bravo::Serializable {
    public:
        std::vector<Signature> signatures;
        CompressionType compression;
        Data packedCFD;
        Data packedTrx;

        PackedTransaction(const Transaction& transaction, CompressionType type) noexcept;

        void serialize(Data& os) const noexcept;
        nlohmann::json serialize() const noexcept;
    };
}} // namespace

// Wrapper for the C interface
struct TWEOSPackedTransaction {
    TW::EOS::PackedTransaction impl;
};