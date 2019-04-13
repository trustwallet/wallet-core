// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

static const uint32_t TWSignatureHashTypeAll = 0x01;
static const uint32_t TWSignatureHashTypeNone = 0x02;
static const uint32_t TWSignatureHashTypeSingle = 0x03;
static const uint32_t TWSignatureHashTypeFork = 0x40;
static const uint32_t TWSignatureHashTypeAnyoneCanPay = 0x80;

extern bool TWSignatureHashTypeIsSingle(uint32_t type);
extern bool TWSignatureHashTypeIsNone(uint32_t type);

enum TWBitcoinSignatureVersion {
    BASE,
    WITNESS_V0
};

TW_EXTERN_C_END
