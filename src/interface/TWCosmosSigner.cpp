// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCosmosSigner.h>

#include "../Cosmos/Signer.h"
#include "../proto/Cosmos.pb.h"

using namespace TW;
using namespace TW::Cosmos;

TW_Cosmos_Proto_SigningOutput TWCosmosSignerSign(TW_Cosmos_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWCosmosSigner{ Signer(std::move(input)) };
    Proto::SigningOutput output = signer->impl.build();

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWCosmosSignerMessage(TW_Cosmos_Proto_SigningInput data, TWData *_Nonnull publicKey) {
    try {
        Proto::SigningInput input;
        input.ParsePartialFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        std::vector<uint8_t> pkVec;
        auto rawPk = TWDataBytes(publicKey);
        pkVec.assign(rawPk, rawPk + static_cast<int>(TWDataSize(publicKey)));

        auto signer = new TWCosmosSigner{Signer(std::move(input))};
        auto serialized = signer->impl.signaturePreimage(pkVec);
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}

TWData *_Nonnull TWCosmosSignerTransaction(TW_Cosmos_Proto_SigningInput data, TWData *_Nonnull pubKey, TWData *_Nonnull signature) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto signer = new TWCosmosSigner{Signer(std::move(input))};

        std::vector<uint8_t> signVec;
        auto rawSign = TWDataBytes(signature);
        signVec.assign(rawSign, rawSign + static_cast<int>(TWDataSize(signature)));

        std::vector<uint8_t> pkVec;
        auto rawPk = TWDataBytes(pubKey);
        pkVec.assign(rawPk, rawPk + static_cast<int>(TWDataSize(pubKey)));
        auto publicKey = PublicKey(pkVec, TWPublicKeyTypeSECP256k1);

        auto encoded = signer->impl.encodeTransaction(signVec, publicKey);
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(encoded.data()),
                                     encoded.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}
