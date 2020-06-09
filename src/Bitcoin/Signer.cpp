// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "TransactionSigner.h"

using namespace TW;
using namespace TW::Bitcoin;

Proto::TransactionPlan Signer::plan(const Proto::SigningInput& input) noexcept {
    auto signer = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    return signer.plan.proto();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    Proto::SigningOutput output;
    auto signer = TransactionSigner<Transaction, TransactionBuilder>(std::move(input));
    auto result = signer.sign();
    if (!result) {
        output.set_error(result.error());
        return output;
    }

    const auto& tx = result.payload();
    *output.mutable_transaction() = tx.proto();

    Data encoded;
    signer.encodeTx(tx, encoded);
    output.set_encoded(encoded.data(), encoded.size());

    Data txHashData = encoded;
    if (tx.hasWitness()) {
        txHashData.clear();
        tx.encode(txHashData, Transaction::SegwitFormatMode::NonSegwit);
    }
    auto txHash = Hash::sha256d(txHashData.data(), txHashData.size());
    std::reverse(txHash.begin(), txHash.end());
    output.set_transaction_id(hex(txHash));
    return output;
}
