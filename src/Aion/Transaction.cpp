// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "RLP.h"

using namespace TW;
using boost::multiprecision::uint128_t;

namespace TW::Aion {

Data Transaction::encode() const noexcept {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(nonce));
    append(encoded, Ethereum::RLP::encode(to.bytes));
    append(encoded, Ethereum::RLP::encode(amount));
    append(encoded, Ethereum::RLP::encode(payload));
    append(encoded, Ethereum::RLP::encode(timestamp));
    append(encoded, RLP::encodeLong(gasLimit));
    append(encoded, RLP::encodeLong(gasPrice));
    append(encoded, RLP::encodeLong(uint128_t(1))); // Aion transaction type
    if (!signature.empty()) {
        append(encoded, Ethereum::RLP::encode(signature));
    }
    return Ethereum::RLP::encodeList(encoded);
}

} // namespace TW::Aion
