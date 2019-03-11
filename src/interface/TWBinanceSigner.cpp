// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBinanceSigner.h>

#include "../Binance/Signer.h"
#include "../proto/Binance.pb.h"

using namespace TW;
using namespace TW::Binance;

TW_Binance_Proto_SigningInput TWBinanceSignerSign(TW_Binance_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    auto signer = new TWBinanceSigner{ Signer(std::move(input)) };
    auto signedData = signer->impl.build();
    return TWDataCreateWithBytes(signedData.data(), signedData.size());
}