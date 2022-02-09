//
// Created by Fitz on 2022/2/7.
//

#include "Signer.h"
#include "Extrinsic.h"

using namespace TW;
using namespace TW::Substrate;

static constexpr size_t hashTreshold = 256;

Data Signer::signaturePreImage(const Proto::SigningInput &input) {
    auto extrinsic = Extrinsic(input);
    auto payload = extrinsic.encodePayload();
    // check if need to hash
    if (payload.size() > hashTreshold) {
        payload = Hash::blake2b(payload, 32);
    }
    return payload;
}

Data Signer::encodeTransaction(const Proto::SigningInput &input, const Data &publicKey, const Data &signature) {
    auto pbk = PublicKey(publicKey, TWPublicKeyTypeED25519);
    auto extrinsic = Extrinsic(input);
    auto encoded = extrinsic.encodeSignature(pbk, signature);
    return encoded;
}