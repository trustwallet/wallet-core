// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Crc {

/// CRC16 implementation compatible with the Stellar version
/// Ported from this implementation: http://introcs.cs.princeton.edu/java/61data/CRC16CCITT.java.html
/// Initial value changed to 0x0000 to match Stellar
uint16_t crc16(uint8_t* bytes, uint32_t length);

uint32_t crc32(const TW::Data& data);

uint32_t crc32C(const TW::Data& data);

} // namespace TW::Crc
