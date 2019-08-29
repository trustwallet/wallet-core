// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../Data.h"

#include <string>

using namespace TW;
using namespace TW::NEAR;

Signer::Signer(Proto::SigningInput&& input) {
    this->input = input;
}

Data Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(transactionData(this->input));
    auto signature = key.sign(hash, TWCurveED25519);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();

    auto signature = sign();
    auto signedTransaction = signedTransactionData(transactionData(this->input), signature);

    output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());

    return output;
}
