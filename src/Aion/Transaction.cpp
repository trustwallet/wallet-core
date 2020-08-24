// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"
#include "Transaction.h"
#include "../Ethereum/RLP.h"

using namespace TW;
using namespace TW::Aion;
using boost::multiprecision::uint128_t;

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
