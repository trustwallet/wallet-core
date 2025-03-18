// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWTransactionUtil.h"
#include "rust/Wrapper.h"

using namespace TW;

TWString* _Nullable TWTransactionUtilCalcTxHash(enum TWCoinType coinType, TWString* _Nonnull encodedTx) {
    try {
        if (encodedTx == nullptr) {
            return nullptr;
        }
        const Rust::TWStringWrapper encodedTxWrapper = TWStringUTF8Bytes(encodedTx);

        const Rust::TWStringWrapper outputDataPtr = Rust::tw_transaction_util_calc_tx_hash(static_cast<uint32_t>(coinType), encodedTxWrapper.get());
        if (!outputDataPtr) {
            return nullptr;
        }

        return TWStringCreateWithUTF8Bytes(outputDataPtr.c_str());
    } catch (...) {
        return nullptr;
    }
}
