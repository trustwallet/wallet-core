// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPolkadotSigner.h>
#include "../Polkadot/Signer.h"

using namespace TW;
using namespace TW::Polkadot;

TWData *_Nonnull TWPolkadotSignerMessage(TWData *_Nonnull data) {
    TW::Polkadot::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto encoded = Signer::signaturePreImage(input);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}

TWData *_Nonnull TWPolkadotSignerTransaction(TWData *_Nonnull data, TWData *_Nonnull publicKey, TWData *_Nonnull signature) {
    TW::Polkadot::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    std::vector<uint8_t> pkVec;
    auto rawPk = TWDataBytes(publicKey);
    pkVec.assign(rawPk, rawPk + static_cast<int>(TWDataSize(publicKey)));

    std::vector<uint8_t> signVec;
    auto rawSign = TWDataBytes(signature);
    signVec.assign(rawSign, rawSign + static_cast<int>(TWDataSize(signature)));

    auto encoded = TW::Polkadot::Signer::encodeTransaction(input, pkVec, signVec);

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}