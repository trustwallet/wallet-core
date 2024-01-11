// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Crc.h"

#include <limits>
#include <string>

using namespace TW;

uint16_t Crc::crc16(uint8_t* bytes, uint32_t length) {
    // Calculate checksum for existing bytes
    uint16_t crc = 0x0000;
    const uint16_t polynomial = 0x1021;

    for (auto i = 0ul; i < length; i++) {
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

// Algorithm inspired by this old-style C implementation:
// https://web.mit.edu/freebsd/head/sys/libkern/crc32.c (Public Domain code)
uint32_t Crc::crc32(const Data& data) {
    uint32_t c = std::numeric_limits<uint32_t>::max();
    for (const auto byte : data) {
        c = crc32_table[(c ^ byte) & 0xFF] ^ (c >> 8);
    }
    return ~c;
}
