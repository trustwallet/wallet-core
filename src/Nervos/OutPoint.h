// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../proto/Nervos.pb.h"

#include <algorithm>
#include <array>
#include <cstring>

namespace TW::Nervos {

/// Nervos transaction out-point reference.
class OutPoint {
public:
    /// The hash of the referenced transaction.
    Data txHash;

    /// The index of the specific output in the transaction.
    uint32_t index;

    OutPoint() = default;

    /// Initializes an out-point reference with hash, index.
    template <typename T>
    OutPoint(const T& h, uint32_t index) {
        txHash.insert(txHash.end(), h.begin(), h.end());
        this->index = index;
    }

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Proto::OutPoint& outPoint) {
        auto& outPointTxHash = outPoint.tx_hash();
        txHash.insert(txHash.end(), outPointTxHash.begin(), outPointTxHash.end());
        index = outPoint.index();
    }

    /// Encodes the out-point into the provided buffer.
    void encode(Data& data) const;

    friend bool operator<(const OutPoint& a, const OutPoint& b) {
        if (a.txHash == b.txHash) {
            return a.index < b.index;
        } else {
            return a.txHash < b.txHash;
        }
    }

    friend bool operator==(const OutPoint& a, const OutPoint& b) {
        return (a.txHash == b.txHash && a.index == b.index);
    }

    friend bool operator!=(const OutPoint& a, const OutPoint& b) { return !(a == b); }

    Proto::OutPoint proto() const {
        auto outPoint = Proto::OutPoint();
        outPoint.set_tx_hash(std::string(txHash.begin(), txHash.end()));
        outPoint.set_index(index);
        return outPoint;
    }
};

} // namespace TW::Nervos
