// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "Hash.h"
#include "HexCoding.h"
#include "../Data.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWBase58.h>
#include <TrezorCrypto/base58.h>

using namespace TW;
using namespace TW::Tezos;

bool Address::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];
    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);

    // verify prefix
    std::array<std::uint8_t, 3> prefix {6, 161, 159};
    for (size_t i = 0; i< prefix.size(); i++) {
        uint8_t byte = buffer[i];
        if (prefix[i] != byte) {
            return false;
        }
    }
    return size == Address::size;
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(data.size() == size);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const std::string& pkey_hash) {
    public_key_hash = pkey_hash;

    size_t capacity = 128;
    uint8_t buffer[capacity];
    int size = base58_decode_check(pkey_hash.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == Address::size);
    std::vector<uint8_t> vec(&buffer[0], &buffer[128]);
    auto str = TW::hex(vec);
    std::copy(buffer, buffer + Address::size, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    auto encoded = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    auto hash = Hash::blake2b(encoded, 20);
    auto data = Data({6, 161, 159});
    append(data, hash);
    assert(data.size() == Address::size);

    size_t s = 0;
    b58enc(nullptr, &s, data.data(), data.size());
    s += 16;

    auto string = std::string();
    string.resize(size);
    base58_encode_check(data.data(), (int)data.size(), HASHER_SHA2D, &string[0], s);

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == Address::size);
    std::vector<uint8_t> vec(&buffer[0], &buffer[128]);
    auto str = TW::hex(vec);
    std::copy(buffer, buffer + Address::size, bytes.begin());
}

std::string Address::string() const {
    size_t size = 0;
    b58enc(nullptr, &size, &bytes, Address::size);
    size += 16;
    std::string public_key_hash(size, ' ');
    base58_encode_check(bytes.data(), Address::size, HASHER_SHA2D, &public_key_hash[0], size);

    std::string result = "";
    if (public_key_hash[0] == 'K') {
        size_t prefixLength = 3;
        uint8_t prefix[3] = {2, 90, 121};
        size_t capacity = 128;
        uint8_t decoded[capacity];

        int decodedLength = checkDecodeAndDropPrefix(public_key_hash, prefixLength, prefix, decoded);
        result += "01";
        result += TW::hex(decoded, decoded + decodedLength);
        result += "00";
    } else {
        result += "00";
        result += forgePublicKeyHash(public_key_hash);
    }
    return result;
}
