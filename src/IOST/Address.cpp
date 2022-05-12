// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Base58.h"

using namespace TW::IOST;
using namespace std;

bool Address::isValid(const std::string& string) {
    const auto parsed = Base58::bitcoin.decode(string);
    return parsed.size() == PublicKey::ed25519Size;
}

Address::Address(const std::string& string) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string!");
    }
    const auto parsed = Base58::bitcoin.decode(string);
    std::copy(std::begin(parsed), std::end(parsed), std::begin(bytes));
}

Address::Address(const PublicKey& publicKey) {
    // copy the raw, compressed key data
    auto data = publicKey.compressed().bytes;
    std::copy(std::begin(data), std::end(data), std::begin(bytes));
}

std::string Address::string() const {
    return Base58::bitcoin.encode(bytes);
}