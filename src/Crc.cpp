// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Crc.h"

#include <boost/crc.hpp>  // for boost::crc_32_type

#include <string>

using namespace TW;

uint16_t Crc::crc16(uint8_t* bytes, uint32_t length) {
    // Calculate checksum for existing bytes
    uint16_t crc = 0x0000;
    const uint16_t polynomial = 0x1021;

    for (auto i = 0; i < length; i++) {
        const auto byte = bytes[i];
        for (auto bitidx = 0; bitidx < 8; bitidx++) {
            const auto bit = ((byte >> (7 - bitidx) & 1) == 1);
            const auto c15 = ((crc >> 15 & 1) == 1);
            crc <<= 1;
            if (c15 ^ bit) {
                crc ^= polynomial;
            }
        }
    }

    return crc & 0xffff;
}

uint32_t Crc::crc32(const Data& data)
{
    boost::crc_32_type result;
    result.process_bytes((const void*)data.data(), data.size());
    return (uint32_t)result.checksum();
}

uint32_t Crc::crc32C(const Data& data)
{
    using crc_32c_type = boost::crc_optimal<32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    crc_32c_type result;
    result.process_bytes((const void*)data.data(), data.size());
    return (uint32_t)result.checksum();
}
