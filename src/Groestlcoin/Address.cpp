// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Base58.h"
#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Groestlcoin;

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string, Hash::HasherGroestl512d);
    if (decoded.size() != Address::size) {
        return false;
    }
    return true;
    // return isValid(string, std::vector<byte>{36, 5});
}

bool Address::isValid(const std::string& string, const std::vector<byte>& validPrefixes) {
    const auto decoded = Base58::bitcoin.decodeCheck(string, Hash::HasherGroestl512d);
    if (decoded.size() != Address::size) {
        return false;
    }
    if (std::find(validPrefixes.begin(), validPrefixes.end(), decoded[0]) == validPrefixes.end()) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::bitcoin.decodeCheck(string, Hash::HasherGroestl512d);
    if (decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey, uint8_t prefix) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Groestlcoin::Address needs a compressed SECP256k1 public key.");
    }
    bytes[0] = prefix;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
}

std::string Address::string() const {
    return Base58::bitcoin.encodeCheck(bytes, Hash::HasherGroestl512d);
}
