// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../Bitcoin/OutPoint.h"
#include "../proto/Bitcoin.pb.h"

#include <algorithm>
#include <array>

namespace TW::Decred {

/// OutPoint defines a Decred data type that is used to track previous
/// transaction outputs.
class OutPoint {
  public:
    /// The hash of the referenced transaction.
    std::array<byte, 32> hash;

    /// The index of the specific output in the transaction.
    uint32_t index;

    /// Which tree the output being spent is in. This is required because there
    /// is more than one tree used to locate transactions in a block.
    int8_t tree;

    OutPoint() = default;

    /// Initializes an out-point reference.
    OutPoint(std::array<byte, 32>&& hash, uint32_t index, int8_t tree)
        : hash(hash), index(index), tree(tree) {}

    /// Initializes an out-point reference.
    OutPoint(const Data& hash, uint32_t index, int8_t tree) : hash(), index(index), tree(tree) {
        std::copy(hash.begin(), hash.end(), this->hash.begin());
    }

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Bitcoin::Proto::OutPoint& other) {
        std::copy(other.hash().begin(), other.hash().begin() + hash.size(), hash.begin());
        index = other.index();
        tree = int8_t(other.tree());
    }

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Bitcoin::OutPoint& other) {
        hash = other.hash;
        index = other.index;
        tree = other.tree;
    }

    /// Encodes the out-point into the provided buffer.
    void encode(Data& data) const;

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

} // namespace TW::Decred
