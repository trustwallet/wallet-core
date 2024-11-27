// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWMessageSigner.h"
#include "rust/Wrapper.h"

using namespace TW;

TWData* _Nullable TWMessageSignerSign(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    Rust::TWDataWrapper inputData(dataIn);
    Rust::TWDataWrapper outputPtr = Rust::tw_message_signer_sign(coin, inputData.get());

    auto outputData = outputPtr.toDataOrDefault();
    if (outputData.empty()) {
        return nullptr;
    }
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}

bool TWMessageSignerVerify(enum TWCoinType coin, TWData* _Nonnull input) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));

    Rust::TWDataWrapper inputData(dataIn);
    return Rust::tw_message_signer_verify(coin, inputData.get());
}
