// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "BitReader.h"

namespace TW::CommonTON {

std::optional<BitReader> BitReader::createExact(const TW::Data &buffer, uint64_t bitLen) {
    Rust::TWDataWrapper twData(buffer);
    auto *readerPtr = Rust::tw_bit_reader_create(twData.get(), bitLen);
    if (!readerPtr) {
        return std::nullopt;
    }

    return BitReader(std::shared_ptr<Rust::TWBitReader>(readerPtr, Rust::tw_bit_reader_delete));
}

std::optional<uint8_t> BitReader::readU8(uint8_t bitCount) {
    Rust::CUInt8ResultWrapper res = Rust::tw_bit_reader_read_u8(reader.get(), bitCount);
    if (res.isErr()) {
        return std::nullopt;
    }
    return res.unwrap().value;
}

std::optional<Data> BitReader::readU8Slice(uint64_t byteCount) {
    Rust::CByteArrayResultWrapper res = Rust::tw_bit_reader_read_u8_slice(reader.get(), byteCount);
    if (res.isErr()) {
        return std::nullopt;
    }
    return res.unwrap().data;
}

bool BitReader::finished() const {
    return Rust::tw_bit_reader_finished(reader.get());
}

} // namespace TW::CommonTON
