// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>

namespace TW::Ontology {

static const uint8_t PUSH0{0x00};
static const uint8_t PUSH_BYTE_33{0x21};
static const uint8_t PUSH_DATA1{0x4C};
static const uint8_t PUSH_DATA2{0x4D};
static const uint8_t PUSH_DATA4{0x4E};
static const uint8_t CHECK_SIG{0xAC};
static const uint8_t CHECK_MULTI_SIG{0xAE};
static const uint8_t PACK{0xC1};
static const uint8_t NEW_STRUCT{0xC6};
static const uint8_t SYS_CALL{0x68};
static const uint8_t DUP_FROM_ALT_STACK{0x6A};
static const uint8_t TO_ALT_STACK{0x6B};
static const uint8_t FROM_ALT_STACK{0x6C};
static const uint8_t SWAP{0x7C};
static const uint8_t HAS_KEY{0xC8};
static const uint8_t APP_CALL{0x67};

} // namespace TW::Ontology
