// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWBitcoinPsbt.h"
#include "Bitcoin/Psbt.h"

using namespace TW;

TWData* _Nonnull TWBitcoinPsbtSign(TWData* _Nonnull input, enum TWCoinType coin) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));
    const auto dataOut = Bitcoin::Psbt::sign(dataIn, coin);
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

TWData* _Nonnull TWBitcoinPsbtPlan(TWData* _Nonnull input, enum TWCoinType coin) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(input));
    const auto dataOut = Bitcoin::Psbt::plan(dataIn, coin);
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}
