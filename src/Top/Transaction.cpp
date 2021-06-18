// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../Ethereum/RLP.h"

using namespace TW;
using namespace TW::Top;

Data Transaction::encode() const noexcept {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(from.bytes));
    append(encoded, Ethereum::RLP::encode(to.bytes));
    append(encoded, Ethereum::RLP::encode(amount));
    append(encoded, Ethereum::RLP::encode(tx_deposit));
    append(encoded, Ethereum::RLP::encode(last_tx_nonce));
    append(encoded, Ethereum::RLP::encode(last_tx_hash));
    append(encoded, Ethereum::RLP::encode(token));
    append(encoded, Ethereum::RLP::encode(note));
    return Ethereum::RLP::encodeList(encoded);
}
