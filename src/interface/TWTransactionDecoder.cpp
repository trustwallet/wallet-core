// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWTransactionDecoder.h"
#include "rust/Wrapper.h"

using namespace TW;

TWData *_Nonnull TWTransactionDecoderDecode(enum TWCoinType coinType, TWData *_Nonnull encodedTx) {
    const Data& txData = *(reinterpret_cast<const Data*>(encodedTx));

    const Rust::TWDataWrapper txDataPtr(txData);
    const Rust::TWDataWrapper outputDataPtr = Rust::tw_transaction_decoder_decode(static_cast<uint32_t>(coinType), txDataPtr.get());

    const auto outputData = outputDataPtr.toDataOrDefault();
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
