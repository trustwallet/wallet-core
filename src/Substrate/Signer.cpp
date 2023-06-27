// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Extrinsic.h"

namespace TW::Substrate {

static constexpr size_t hashTreshold = 256;

Data Signer::signaturePreImage(const Proto::SigningInput& input) {
    auto extrinsic = Extrinsic(input);
    auto payload = extrinsic.encodePayload();
    // check if need to hash
    if (payload.size() > hashTreshold) {
        payload = Hash::blake2b(payload, 32);
    }
    return payload;
}

Data Signer::encodeTransaction(const Proto::SigningInput& input, const Data& publicKey, const Data& signature) {
    auto pbk = PublicKey(publicKey, TWPublicKeyTypeED25519);
    auto extrinsic = Extrinsic(input);
    auto encoded = extrinsic.encodeSignature(pbk, signature);
    return encoded;
}

} // namespace TW::Substrate
