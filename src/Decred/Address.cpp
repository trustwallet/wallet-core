// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include "../Hash.h"

#include <stdexcept>

using namespace TW;
using namespace TW::Decred;

static const std::array<byte, 2> prefix = {0x07, 0x3f};
static const auto keyhashSize = Hash::ripemdSize;
static const auto addressDataSize = keyhashSize + 2;

bool Address::isValid(const std::string& string) noexcept {
    const auto data = Base58::bitcoin.decodeCheck(string, Hash::blake256d);
    if (data.size() != addressDataSize) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    const auto data = Base58::bitcoin.decodeCheck(string, Hash::blake256d);
    if (data.size() != addressDataSize) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(data.begin() + 2, data.end(), keyhash.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type() != PublicKeyType::secp256k1) {
        throw std::invalid_argument("Invalid publid key type");
    }
    const auto hash = Hash::ripemd(Hash::blake256(publicKey.bytes));
    std::copy(hash.begin(), hash.end(), keyhash.begin());
}

std::string Address::string() const {
    auto addressPreimage = Data();
    addressPreimage.reserve(addressDataSize);
    addressPreimage.insert(addressPreimage.end(), prefix.begin(), prefix.end());
    addressPreimage.insert(addressPreimage.end(), keyhash.begin(), keyhash.end());

    return Base58::bitcoin.encodeCheck(addressPreimage, Hash::blake256d);
}
