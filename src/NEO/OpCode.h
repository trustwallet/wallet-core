// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>

namespace TW::NEO {

static const uint8_t PUSHBYTES1{0x01};
static const uint8_t PUSHBYTES21{0x21};
static const uint8_t PUSHBYTES40{0x40};
static const uint8_t PUSHBYTES75{0x4B};
static const uint8_t PUSHDATA1{0x4C};
static const uint8_t PUSHDATA2{0x4D};
static const uint8_t PUSHDATA4{0x4E};
static const uint8_t PUSH0{0x00};
static const uint8_t PUSH1{0x51};
static const uint8_t PUSH2{0x52};
static const uint8_t PUSH3{0x53};
static const uint8_t PUSH5{0x55};
static const uint8_t RET{0x66};
static const uint8_t APPCALL{0x67};
static const uint8_t CHECKSIG{0xAC};
static const uint8_t PACK{0xC1};
static const uint8_t THROWIFNOT{0xF1};

} // namespace TW::NEO
