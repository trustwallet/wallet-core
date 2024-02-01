// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include "../Base58.h"
#include "../Coin.h"
#include "../Hash.h"

#include <stdexcept>

namespace TW::Decred {

static const auto keyhashSize = Hash::ripemdSize;
static const auto addressDataSize = keyhashSize + 2;

bool Address::isValid(const std::string& string) noexcept {
    const auto data = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherBlake256d);
    if (data.size() != addressDataSize) {
        return false;
    }
    if (data[0] != TW::staticPrefix(TWCoinTypeDecred)) {
        return false;
    }

    return (data[1] == TW::p2pkhPrefix(TWCoinTypeDecred) ||
            data[1] == TW::p2shPrefix(TWCoinTypeDecred));
}

Address::Address(const std::string& string) {
    const auto data = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherBlake256d);
    if (data.size() != addressDataSize) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto hash = Hash::ripemd(Hash::blake256(publicKey.bytes));
    std::copy(hash.begin(), hash.end(), bytes.begin() + 2);
    bytes[0] = TW::staticPrefix(TWCoinTypeDecred);
    bytes[1] = TW::p2pkhPrefix(TWCoinTypeDecred);
}

std::string Address::string() const {
    return Base58::encodeCheck(bytes, Rust::Base58Alphabet::Bitcoin, Hash::HasherBlake256d);
}

} // namespace TW::Decred
