// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWTransactionUtil.h"
#include "rust/Wrapper.h"

#include <iostream>

using namespace TW;

TWString* _Nonnull TWTransactionUtilCalcTxHash(enum TWCoinType coinType, TWString *_Nonnull encodedTx) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);
    const Rust::TWStringWrapper encodedTxStr = encodedTxRef;

    const Rust::TWStringWrapper outputDataPtr = Rust::tw_transaction_util_calc_tx_hash(static_cast<uint32_t>(coinType), encodedTxStr.get());

    return TWStringCreateWithUTF8Bytes(outputDataPtr.toStringOrDefault().c_str());
}
