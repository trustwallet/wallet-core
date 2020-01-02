// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../PrivateKey.h"
#include "Serialization.h"

#include "../Data.h"
#include "../Hash.h"

using namespace TW;
using namespace TW::Cosmos;

Data Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto preimage = signaturePreimage(input).dump();
    auto hash = Hash::sha256(preimage);
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return Data(signature.begin(), signature.end() - 1);
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();
    auto signature = sign();
    auto txJson = transactionJSON(input, signature);
    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());
    return output;
}
