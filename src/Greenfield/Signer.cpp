// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../PublicKey.h"

namespace TW::Greenfield {

Cosmos::Proto::SigningOutput Signer::sign(const Cosmos::Proto::SigningInput &input) {
    // TODO: Check and finalize implementation

    auto protoOutput = Cosmos::Proto::SigningOutput();
    Data encoded;
    // auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    // auto signature = privateKey.sign(payload, TWCurveED25519);
    // encoded = encodeSignature(signature);

    // protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

TxCompiler::Proto::PreSigningOutput Signer::preImageHashes(const Cosmos::Proto::SigningInput& input) {
}

} // namespace TW::Greenfield
