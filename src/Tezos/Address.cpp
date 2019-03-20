// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "Forging.h"

#include "../Base58.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <TrezorCrypto/ecdsa.h>

using namespace TW;
using namespace TW::Tezos;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    // verify prefix
    std::array<byte, 3> prefix {6, 161, 159};
    if (!std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    // TODO: isValid(bytes)
    if (data.size() != size) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto publicKeySize = publicKey.ed25519Size;

    // Drop first byte of the public key which is a tag.
    auto encoded = Data(publicKey.bytes.begin() + 1, publicKey.bytes.begin()  + publicKeySize);
    auto hash = Hash::blake2b(encoded, 20);
    auto addressData = Data({6, 161, 159});
    append(addressData, hash);
    if (addressData.size() != Address::size)
        throw std::invalid_argument("Invalid address key data");
    std::copy(addressData.data(), addressData.data() + Address::size, bytes.begin());
}

std::string Address::string() const {
    return Base58::bitcoin.encodeCheck(bytes);
}

Data Address::forge() const {
    auto data = Data();
    std::string s = string();

    if (s[0] == 'K') {
        std::array<byte, 3> prefix = {2, 90, 121};
        const auto decoded = Base58::bitcoin.decodeCheck(s);
        if (decoded.size() != 23 || !std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
            throw std::invalid_argument("Invalid Address For forge");
        }
        data.push_back(0x01);
        data.insert(data.end(), decoded.begin() + prefix.size(), decoded.end());
        data.push_back(0x00);
        return data;
    }
    data.push_back(0);
    append(data, forgePublicKeyHash(s));
    return data;
}
