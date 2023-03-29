// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Bitcoin SIGHASH type.
TW_EXPORT_ENUM(uint32_t)
enum TWBitcoinSigHashType {
    TWBitcoinSigHashTypeAll = 0x01,
    TWBitcoinSigHashTypeNone = 0x02,
    TWBitcoinSigHashTypeSingle = 0x03,
    TWBitcoinSigHashTypeFork = 0x40,
    TWBitcoinSigHashTypeForkBTG = 0x4f40,
    TWBitcoinSigHashTypeAnyoneCanPay = 0x80
};

/// Determines if the given sig hash is single
///
/// \param type sig hash type
/// \return true if the sigh hash type is single, false otherwise
TW_EXPORT_METHOD
bool TWBitcoinSigHashTypeIsSingle(enum TWBitcoinSigHashType type);

/// Determines if the given sig hash is none
///
/// \param type sig hash type
/// \return true if the sigh hash type is none, false otherwise
TW_EXPORT_METHOD
bool TWBitcoinSigHashTypeIsNone(enum TWBitcoinSigHashType type);

TW_EXTERN_C_END
