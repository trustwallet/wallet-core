// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32.h"
#include "Hash.h"

#include "Address.h"

using namespace TW;
using namespace TW::IoTeX;

bool Address::isValid(const std::string& addr) {
    if (addr.size() != 41 || addr[0] != 'i' || addr[1] != 'o') {
        return false;
    }

    const auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return false;
    }
    if (dec.first != "io") {
        return false;
    }

    Data keyHash;
    auto success = Bech32::convertBits<5, 8, false>(keyHash, dec.second);
    if (!success || keyHash.size() != 20) {
        return false;
    }
    return true;
}

Address::Address(const std::string& addr) {
    if (!isValid(addr)) {
        throw std::invalid_argument("IoTeX: Invalid address data");
    }

    const auto dec = Bech32::decode(addr);
    Bech32::convertBits<5, 8, false>(keyHash, dec.second);
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("IoTeX: Address needs an extended SECP256k1 public key");
    }
    keyHash = publicKey.hash({}, static_cast<Data(*)(const byte*, const byte*)>(Hash::keccak256), true);
    keyHash.erase(keyHash.begin(), keyHash.begin() + 12);
}

Address::Address(const Data& kh) {
    if (!isValid(kh)) {
        throw std::invalid_argument("IoTeX: Invalid address data");
    }
    keyHash.resize(20);
    std::copy(kh.begin(), kh.end(), keyHash.begin());
}

std::string Address::string() const {
    Data enc;
    Bech32::convertBits<8, 5, true>(enc, keyHash);
    return Bech32::encode("io", enc);
}
