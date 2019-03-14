// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Hash.h"

using namespace TW;
using namespace TW::Aion;

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    auto encoded = transaction.encode();
    auto hashData = Hash::blake2b(encoded, 32);
    auto hashSignature = privateKey.sign(hashData, TWCurveEd25519);
    auto publicKeyData = privateKey.getPublicKey(PublicKeyType::ed25519).bytes;

    // Aion signature = pubKeyBytes + signatureBytes
    Data result(publicKeyData.begin() + 1, publicKeyData.end());
    result.insert(result.end(), hashSignature.begin(), hashSignature.end());

    transaction.signature = result;
}
