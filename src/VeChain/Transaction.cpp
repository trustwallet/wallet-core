// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../Ethereum/RLP.h"

namespace TW::VeChain {

using RLP = Ethereum::RLP;

Data encode(const Clause& clause) noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(clause.to.bytes));
    append(encoded, RLP::encode(clause.value));
    append(encoded, RLP::encode(clause.data));
    return RLP::encodeList(encoded);
}

Data encodeClauses(std::vector<Clause> clauses) noexcept {
    auto encoded = Data();
    for (const auto& clause : clauses) {
        auto encodedClause = encode(clause);
        append(encoded, encodedClause);
    }
    return RLP::encodeList(encoded);
}

Data Transaction::encode() const noexcept {
    auto encoded = Data();
    append(encoded, RLP::encode(chainTag));
    append(encoded, RLP::encode(blockRef));
    append(encoded, RLP::encode(expiration));
    append(encoded, encodeClauses(clauses));
    append(encoded, RLP::encode(gasPriceCoef));
    append(encoded, RLP::encode(gas));
    append(encoded, RLP::encode(dependsOn));
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encodeList(reserved));
    if (!signature.empty()) {
        append(encoded, RLP::encode(signature));
    }
    return RLP::encodeList(encoded);
}

} // namespace TW::VeChain
