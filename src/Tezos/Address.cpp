// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "Forging.h"

#include "../HexCoding.h"
#include "../Hash.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

using namespace TW::Tezos;

bool Address::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];
    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);

    // verify prefix
    std::array<std::uint8_t, 3> prefix {6, 161, 159};
    for (size_t i = 0; i< prefix.size(); i++)
        if (prefix[i] != (uint8_t) buffer[i])
            return false;
    return size == Address::size;
}

Address::Address(const std::string& string) {
    uint8_t decoded[128];
    auto size = base58CheckDecodePrefix(string, 0, nullptr, decoded);
    if (size != Address::size)
        throw std::invalid_argument("Invalid address key data");
    std::copy(decoded, decoded + Address::size, bytes.begin());
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
    return bytesToBase58(bytes.data(), Address::size);
}

std::string Address::forge() const {
    std::string s = string();

    if (s[0] == 'K') {
        size_t prefixLength = 3;
        uint8_t prefix[3] = {2, 90, 121};
        uint8_t decoded[128];

        int decodedLength = base58CheckDecodePrefix(s, prefixLength, prefix, decoded);
        if (decodedLength != 20)
            throw std::invalid_argument("Invalid Address For forge");
        return "01" + TW::hex(decoded, decoded + decodedLength) + "00";
    }
    return "00" + forgePublicKeyHash(s);
}
