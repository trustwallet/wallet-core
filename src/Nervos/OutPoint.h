// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../proto/Nervos.pb.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <cstring>

namespace TW::Nervos {

/// Nervos transaction out-point reference.
struct OutPoint {
    /// The hash of the referenced transaction.
    Data txHash;

    /// The index of the specific output in the transaction.
    uint32_t index;

    OutPoint() = default;

    /// Initializes an out-point reference with hash, index.
    template <typename T>
    OutPoint(const T& h, uint32_t index) : txHash(std::begin(h), std::end(h)), index(index) {}

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Proto::OutPoint& outPoint)
        : txHash(std::begin(outPoint.tx_hash()), std::end(outPoint.tx_hash()))
        , index(outPoint.index()) {}

    /// Encodes the out-point into the provided buffer.
    void encode(Data& data) const;
    nlohmann::json json() const;

    friend bool operator==(const OutPoint& lhs, const OutPoint& rhs) {
        return (lhs.txHash == rhs.txHash && lhs.index == rhs.index);
    }

    Proto::OutPoint proto() const {
        auto outPoint = Proto::OutPoint();
        outPoint.set_tx_hash(std::string(txHash.begin(), txHash.end()));
        outPoint.set_index(index);
        return outPoint;
    }
};

} // namespace TW::Nervos
