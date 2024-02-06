// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWEthereumRlp.h>

#include "rust/Wrapper.h"
#include "Data.h"

using namespace TW;

TWData* _Nonnull TWEthereumRlpEncode(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    const Rust::TWDataWrapper dataInPtr(dataIn);
    Rust::TWDataWrapper dataOutPtr = Rust::tw_ethereum_rlp_encode(static_cast<uint32_t>(coin), dataInPtr.get());

    auto dataOut = dataOutPtr.toDataOrDefault();
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}
