// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>

#include "Coin.h"

//#pragma clang diagnostic push
//#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;

/*
template <typename Signer, typename Input>
TWData* _Nonnull AnySign(TWData* _Nonnull data) {
    auto input = Input();
    input.ParseFromArray(TWDataBytes(data), (int)TWDataSize(data));
    auto serialized = Signer::sign(input).SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}
*/

TWData* _Nonnull TWAnySignerSign(TWData* _Nonnull data, enum TWCoinType coin) {
    const Data& dataIn = *(reinterpret_cast<const Data*>(data));
    Data dataOut;
    TW::anyCoinSign(coin, dataIn, dataOut);
    return TWDataCreateWithBytes(dataOut.data(), dataOut.size());
}

//#pragma clang diagnostic pop
