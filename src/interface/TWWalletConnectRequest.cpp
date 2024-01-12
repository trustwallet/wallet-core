// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWWalletConnectRequest.h>

#include "rust/Wrapper.h"

using namespace TW;

TWData* _Nonnull TWWalletConnectRequestParse(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& inputData = *reinterpret_cast<const Data*>(input);
    Rust::TWDataWrapper twInputData = inputData;

    Rust::TWDataWrapper twOutputData = Rust::tw_wallet_connect_request_parse(static_cast<uint32_t>(coin), twInputData.get());
    auto outputData = twOutputData.toDataOrDefault();
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
