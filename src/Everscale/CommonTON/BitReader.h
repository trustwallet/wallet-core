// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/Wrapper.h"

namespace TW::CommonTON {

class BitReader {
public:
    // Tries to create a bit reader with exact `bitLen` number of bits.
    static std::optional<BitReader> createExact(const Data& buffer, uint64_t bitLen);

    // Read at most 8 bits into a u8.
    std::optional<uint8_t> readU8(uint8_t bitCount);

    // Reads an entire slice of `byteCount` bytes. If there aren't enough bits remaining
    // after the internal cursor's current position, returns none.
    std::optional<Data> readU8Slice(uint64_t byteCount);

    bool finished() const;

private:
    explicit BitReader(std::shared_ptr<Rust::TWBitReader> reader): reader(std::move(reader)) {}

    std::shared_ptr<Rust::TWBitReader> reader;
};

} // namespace TW::CommonTON
