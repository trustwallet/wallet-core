// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Extrinsic.h"
#include "../Hash.h"
#include "../PrivateKey.h"

using namespace TW;
using namespace TW::Polkadot;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto extrinsic = Extrinsic(input);
    auto hash = Hash::blake2b(extrinsic.getPreImage(), 64);
    auto signature = privateKey.sign(hash, TWCurveED25519);
    auto encoded = extrinsic.encodeSignature(signature);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}
