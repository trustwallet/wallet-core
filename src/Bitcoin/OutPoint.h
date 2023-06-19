// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "algorithm/to_array.h"
#include "Data.h"
#include "../proto/Bitcoin.pb.h"

#include <algorithm>
#include <array>
#include <cstring>

namespace TW::Bitcoin {

/// Bitcoin transaction out-point reference.
struct OutPoint {
    /// The hash of the referenced transaction.
    std::array<byte, 32> hash;

    /// The index of the specific output in the transaction.
    uint32_t index;

    /// Sequence number, matches sequence from Proto::OutPoint (not always used, see also
    /// TransactionInput.sequence)
    uint32_t sequence;

    /// The tree in utxo, only works for DCR
    int8_t tree;

    OutPoint() noexcept = default;

    /// Initializes an out-point reference with hash, index.
    template <typename T>
    OutPoint(const T& h, uint32_t index, uint32_t sequence = 0, int8_t tree = 0) noexcept
        : hash(to_array<byte, 32>(h)), index(index), sequence(sequence), tree(tree) {}

    /// Initializes an out-point from a Protobuf out-point.
    OutPoint(const Proto::OutPoint& other) noexcept
        : OutPoint(other.hash(), other.index(), other.sequence(), int8_t(other.tree())) {
        assert(other.hash().size() == 32);
    }

    /// Encodes the out-point into the provided buffer.
    void encode(Data& data) const noexcept;

    Proto::OutPoint proto() const {
        auto op = Proto::OutPoint();
        op.set_hash(std::string(hash.begin(), hash.end()));
        op.set_index(index);
        op.set_sequence(sequence);
        op.set_tree(int32_t(tree));
        return op;
    }
};

} // namespace TW::Bitcoin
