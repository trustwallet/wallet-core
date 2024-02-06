// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Solana/VersionedTransaction.h"
#include "Solana/Encoding.h"

namespace TW::Solana {

std::string VersionedTransaction::serialize() const {
    Data buffer;

    append(buffer, shortVecLength<Data>(this->signatures));
    for (auto &&signature : this->signatures) {
        append(buffer, signature);
    }
    append(buffer, this->messageData());

    return Base58::encode(buffer);
}

Data VersionedTransaction::messageData() const {
    return Solana::serialize(this->message);
}

uint8_t VersionedTransaction::getAccountIndex(Address publicKey) {
    const auto accountKeys = Solana::accountKeys(this->message);
    auto item = std::find(accountKeys.begin(), accountKeys.end(), publicKey);
    if (item == accountKeys.end()) {
        throw std::invalid_argument("publicKey not found in message.accountKeys");
    }
    return (uint8_t)std::distance(accountKeys.begin(), item);
}
} // namespace TW::Solana
