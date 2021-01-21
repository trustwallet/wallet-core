// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Avalanche;


Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    // TODO: Check and finalize implementation

    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    // auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    // auto signature = privateKey.sign(payload, TWCurveED25519);
    // encoded = encodeSignature(signature);
    // EJR Note: Basically, `encoded` needs to be filled with whatever bytes get sent to the network, so the full SignedTransaction


    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}


Proto::SigningOutput Signer::plan(const Proto::SigningInput &input) noexcept {
    // TODO: Check and finalize implementation

    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    // auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    // auto signature = privateKey.sign(payload, TWCurveED25519);
    // encoded = encodeSignature(signature);
    // EJR Note: Basically, `encoded` needs to be filled with whatever bytes get sent to the network, so the full SignedTransaction


    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}