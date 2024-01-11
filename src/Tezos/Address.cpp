// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "BinaryCoding.h"
#include "Forging.h"

#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>

namespace TW::Tezos {

/// Address prefixes.
const std::array<TW::byte, 3> tz1Prefix{6, 161, 159};
const std::array<TW::byte, 3> tz2Prefix{6, 161, 161};
const std::array<TW::byte, 3> tz3Prefix{6, 161, 164};
const std::array<TW::byte, 3> kt1Prefix{2, 90, 121};

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    // verify prefix
    if (std::equal(tz1Prefix.begin(), tz1Prefix.end(), decoded.begin()) ||
        std::equal(tz2Prefix.begin(), tz2Prefix.end(), decoded.begin()) ||
        std::equal(tz3Prefix.begin(), tz3Prefix.end(), decoded.begin())) {
        return true;
    }

    // contract prefix
    if (std::equal(kt1Prefix.begin(), kt1Prefix.end(), decoded.begin())) {
        return true;
    }

    return false;
}

Address::Address(const PublicKey& publicKey) {
    auto encoded = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    auto hash = Hash::blake2b(encoded, 20);
    Data addressData;
    if (publicKey.type == TWPublicKeyTypeSECP256k1) {
        addressData = Data({6, 161, 161});
    } else if (publicKey.type == TWPublicKeyTypeED25519){
        addressData = Data({6, 161, 159});
    } else {
        throw std::invalid_argument("unsupported public key type");
    }
    append(addressData, hash);
    if (addressData.size() != Address::size)
        throw std::invalid_argument("Invalid address key data");
    std::copy(addressData.data(), addressData.data() + Address::size, bytes.begin());
}

std::string Address::deriveOriginatedAddress(const std::string& operationHash, int operationIndex) {
    // Decode and remove 2 byte prefix.
    auto decoded = Base58::decodeCheck(operationHash);
    decoded.erase(decoded.begin(), decoded.begin() + 2);
    TW::encode32BE(operationIndex, decoded);

    auto hash = Hash::blake2b(decoded, 20);

    auto prefix = Data({2, 90, 121});
    prefix.insert(prefix.end(), hash.begin(), hash.end());

    return Base58::encodeCheck(prefix);
}

Data Address::forgePKH() const {
    std::string s = string();
    return forgePublicKeyHash(s);
}

Data Address::forge() const {
    // normal address
    // https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L183
    if (std::equal(tz1Prefix.begin(), tz1Prefix.end(), bytes.begin()) ||
        std::equal(tz2Prefix.begin(), tz2Prefix.end(), bytes.begin()) ||
        std::equal(tz3Prefix.begin(), tz3Prefix.end(), bytes.begin())) {
        std::string s = string();
        Data forgedPKH = forgePublicKeyHash(s);
        Data forged = Data();
        forged.insert(forged.end(), 0x00);
        forged.insert(forged.end(), forgedPKH.begin(), forgedPKH.end());
        return forged;
    }

    // contract address
    // https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L183
    if (std::equal(kt1Prefix.begin(), kt1Prefix.end(), bytes.begin())) {
        std::string s = string();
        Data forgedPrefix = forgePrefix(kt1Prefix, s);
        Data forged = Data();
        forged.insert(forged.end(), 0x01);
        forged.insert(forged.end(), forgedPrefix.begin(), forgedPrefix.end());
        forged.insert(forged.end(), 0x00);
        return forged;
    }

    throw std::invalid_argument("invalid address");
}

} // namespace TW::Tezos
