// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Bitcoin/TransactionSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "TransactionBuilder.h"

namespace TW::Zcash {

TransactionPlan Signer::plan(const SigningInput& input) noexcept {
    auto plan = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

SigningOutput Signer::sign(const SigningInput& input) noexcept {
    SigningOutput output;
    auto result = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::sign(input);
    if (!result) {
        output.set_error(result.error());
    } else {
        const auto& tx = result.payload();
        *output.mutable_transaction() = tx.proto();

        Data encoded;
        tx.encode(encoded);
        output.set_encoded(encoded.data(), encoded.size());

        auto txHash = Hash::sha256d(encoded.data(), encoded.size());
        std::reverse(txHash.begin(), txHash.end());
        output.set_transaction_id(hex(txHash));
    }
    return output;
}

} // namespace TW::Zcash
