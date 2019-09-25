// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/NULS.pb.h"

#include <algorithm>

namespace TW::NULS {

struct TransactionBuilder {
    static const uint16_t TRANSACTION_FIX_SIZE = 11; //type 2, time 4, txData 1, hash 4
    static const uint16_t TRANSACTION_SIG_MAX_SIZE = 110;
    static const uint16_t TRANSACTION_INPUT_SIZE = 70;
    static const uint16_t TRANSACTION_OUTPUT_SIZE = 70;
    /// Transaction size must less that 300KB
    static const uint64_t MAX_TRANSACTION_SIZE = 300 * 1024;
    /// 0.001 NULS per KB
    static const uint64_t MIN_PRICE_PRE_1024_BYTES = 100000;

    static int32_t calculatorMaxInput(uint32_t remarkSize) {
        uint32_t outputSize = 1 * TRANSACTION_OUTPUT_SIZE;
        uint32_t maxInputs =
                (MAX_TRANSACTION_SIZE - TRANSACTION_FIX_SIZE - TRANSACTION_SIG_MAX_SIZE - remarkSize - outputSize) / TRANSACTION_INPUT_SIZE;
        if ((MAX_TRANSACTION_SIZE - TRANSACTION_FIX_SIZE - TRANSACTION_SIG_MAX_SIZE - remarkSize - outputSize) % TRANSACTION_INPUT_SIZE != 0) {
            maxInputs -= 1;
        }
        return maxInputs;
    }

    static uint32_t calculatorTransactionSize(uint32_t inputCount, uint32_t outputCount,
                                              uint32_t remarkSize) {
        uint32_t size = TRANSACTION_FIX_SIZE + TRANSACTION_SIG_MAX_SIZE + TRANSACTION_INPUT_SIZE * inputCount +
                        TRANSACTION_OUTPUT_SIZE * outputCount + remarkSize;
        return size;
    }

    static inline uint64_t calculatorTransactionFee(uint64_t size) {
        uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
        if (size % 1024 > 0) {
            fee += MIN_PRICE_PRE_1024_BYTES;
        }
        return fee;
    }
};

} // namespace TW::NULS
