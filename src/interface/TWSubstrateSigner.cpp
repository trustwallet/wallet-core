//
// Created by Fitz on 2022/2/7.
//

#include <TrustWalletCore/TWSubstrateSigner.h>
#include "../Substrate/Signer.h"

using namespace TW;
using namespace TW::Substrate;

TWData *_Nonnull TWSubstrateSignerMessage(TW_Substrate_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto encoded = Signer::signaturePreImage(input);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());;
}

TWData *_Nonnull TWSubstrateSignerTransaction(TW_Substrate_Proto_SigningInput data, TWData *_Nonnull publicKey, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    std::vector<uint8_t> pkVec;
    auto rawPk = TWDataBytes(publicKey);
    pkVec.assign(rawPk, rawPk + static_cast<int>(TWDataSize(publicKey)));

    std::vector<uint8_t> signVec;
    auto rawSign = TWDataBytes(signature);
    signVec.assign(rawSign, rawSign + static_cast<int>(TWDataSize(signature)));

    auto encoded = Signer::encodeTransaction(input, pkVec, signVec);

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}