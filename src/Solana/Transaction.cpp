// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"
#include "Solana/Encoding.h"

#include "Hash.h"
#include "Signer.h"

#include <vector>

namespace TW::Solana {

std::string Transaction::serialize() const {
    Data buffer;

    append(buffer, shortVecLength<Data>(this->signatures));
    for (auto &&signature : this->signatures) {
        append(buffer, signature);
    }
    append(buffer, this->messageData());

    return Base58::encode(buffer);
}

Data Transaction::messageData() const {
    return this->message.serialize();
}

uint8_t Transaction::getAccountIndex(Address publicKey) {
    auto item =
        std::find(this->message.accountKeys.begin(), this->message.accountKeys.end(), publicKey);
    if (item == this->message.accountKeys.end()) {
        throw std::invalid_argument("publicKey not found in message.accountKeys");
    }
    return (uint8_t)std::distance(this->message.accountKeys.begin(), item);
}

} // namespace TW::Solana
