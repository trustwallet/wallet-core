// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../proto/Bitcoin.pb.h"

#include <algorithm>
#include <array>
#include <cstring>

namespace TW::Bitcoin {

/// Bitcoin transaction out-point reference.
class OutPoint {
  public:
    /// The hash of the referenced transaction.
    std::array<byte, 32> hash;

    /// The index of the specific output in the transaction.
    uint32_t index;

    /// Initializes an out-point reference with a hash and an index.
    template <typename T>
    OutPoint(const T& h, uint32_t index) {
        std::copy(std::begin(h), std::end(h), hash.begin());
        this->index = index;
    }

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Proto::OutPoint& other) {
        assert(other.hash().size() == 32);
        std::copy(other.hash().begin(), other.hash().end(), hash.begin());
        index = other.index();
    }

    /// Encodes the out-point into the provided buffer.
    void encode(std::vector<uint8_t>& data) const;

    friend bool operator<(const OutPoint& a, const OutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return cmp < 0 || (cmp == 0 && a.index < b.index);
    }

    friend bool operator==(const OutPoint& a, const OutPoint& b) {
        int cmp = std::memcmp(a.hash.data(), b.hash.data(), 32);
        return (cmp == 0 && a.index == b.index);
    }

    friend bool operator!=(const OutPoint& a, const OutPoint& b) { return !(a == b); }
};

} // namespace TW::Bitcoin
