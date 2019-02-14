// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBinanceSigner.h>

#include "../Binance/Signer.h"
#include "../proto/Binance.pb.h"

using namespace TW::Binance;

struct TWBinanceSigner *_Nonnull TWBinanceSignerCreate(TW_Binance_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));
    return new TWBinanceSigner{ Signer(std::move(input)) };
}

void TWBinanceSignerDelete(struct TWBinanceSigner *_Nonnull signer) {
    delete signer;
}

TWData *_Nullable TWBinanceSignerBuild(struct TWBinanceSigner *_Nonnull signer) {
    auto data = signer->impl.build();
    if (data.empty()) {
        return nullptr;
    }
    return TWDataCreateWithBytes(data.data(), data.size());
}
