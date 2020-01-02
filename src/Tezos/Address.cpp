// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "Forging.h"

#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>

using namespace TW;
using namespace TW::Tezos;

/// Address prefixes.
const std::array<byte, 3> tz1Prefix{6, 161, 159};
const std::array<byte, 3> tz2Prefix{6, 161, 161};
const std::array<byte, 3> tz3Prefix{6, 161, 164};

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    // verify prefix
    if (std::equal(tz1Prefix.begin(), tz1Prefix.end(), decoded.begin()) ||
        std::equal(tz2Prefix.begin(), tz2Prefix.end(), decoded.begin()) ||
        std::equal(tz3Prefix.begin(), tz3Prefix.end(), decoded.begin())) {
        return true;
    }

    return false;
}

Address::Address(const PublicKey& publicKey) {
    auto encoded = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    auto hash = Hash::blake2b(encoded, 20);
    auto addressData = Data({6, 161, 159});
    append(addressData, hash);
    if (addressData.size() != Address::size)
        throw std::invalid_argument("Invalid address key data");
    std::copy(addressData.data(), addressData.data() + Address::size, bytes.begin());
}

std::string Address::deriveOriginatedAddress(const std::string& operationHash, int operationIndex) {
    // Decode and remove 2 byte prefix.
    auto decoded = Base58::bitcoin.decodeCheck(operationHash);
    decoded.erase(decoded.begin(), decoded.begin() + 2);
    TW::encode32BE(operationIndex, decoded);

    auto hash = Hash::blake2b(decoded, 20);

    auto prefix = Data({2, 90, 121});
    prefix.insert(prefix.end(), hash.begin(), hash.end());

    return Base58::bitcoin.encodeCheck(prefix);
}

Data Address::forge() const {
    std::string s = string();
    return forgePublicKeyHash(s);
}
