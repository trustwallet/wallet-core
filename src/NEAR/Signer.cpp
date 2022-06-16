// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../PrivateKey.h"

using namespace TW;
using namespace TW::NEAR;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto transaction = transactionData(input);
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(transaction);
    auto signature = key.sign(hash, TWCurveED25519);
    auto output = Proto::SigningOutput();
    auto signedTransaction = signedTransactionData(transaction, signature);
    output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());
    output.set_hash(hash.data(), hash.size());
    return output;
}
