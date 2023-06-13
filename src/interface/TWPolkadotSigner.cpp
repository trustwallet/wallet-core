//
// Created by Fitz on 2021/7/8.
//

#include <TrustWalletCore/TWPolkadotSigner.h>
#include "../Polkadot/Signer.h"
#include "../proto/Polkadot.pb.h"

using namespace TW;
using namespace TW::Polkadot;

TWData *_Nonnull TWPolkadotSignerMessage(TW_Polkadot_Proto_SigningInput data) {
    TW::Polkadot::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto encoded = Signer::signaturePreImage(input);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());;
}

TWData *_Nonnull TWPolkadotSignerTransaction(TW_Polkadot_Proto_SigningInput data, TWData *_Nonnull publicKey, TWData *_Nonnull signature) {
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