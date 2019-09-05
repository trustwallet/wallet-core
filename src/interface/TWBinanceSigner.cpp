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

TW_Binance_Proto_SigningOutput TWBinanceSignerSign(TW_Binance_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWBinanceSigner{ Signer(std::move(input)) };
    auto encoded = signer->impl.build();
    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWBinanceMessage(TW_Binance_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWBinanceSigner{ Signer(std::move(input)) };
    auto encoded = signer->impl.signaturePreimage();
    return encoded.c_str();
}

TWData *_Nonnull TWBinanceTransaction(TW_Binance_Proto_SigningInput data, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWBinanceSigner{ Signer(std::move(input)) };
    std::vector<uint8_t> signVec;
    auto rawSign = TWDataBytes(signature);
    signVec.assign(rawSign, rawSign + static_cast<int>(TWDataSize(signature)));
    auto encoded = signer->impl.encodeTransaction(signVec);

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}