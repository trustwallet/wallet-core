// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include "../Base58.h"
#include <TrezorCrypto/ecdsa.h>

namespace TW::Groestlcoin {

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherGroestl512d);
    if (decoded.size() != Address::size) {
        return false;
    }
    return true;
}

bool Address::isValid(const std::string& string, const std::vector<byte>& validPrefixes) {
    const auto decoded = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherGroestl512d);
    if (decoded.size() != Address::size) {
        return false;
    }
    if (std::find(validPrefixes.begin(), validPrefixes.end(), decoded[0]) == validPrefixes.end()) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherGroestl512d);
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
    return Base58::encodeCheck(bytes, Rust::Base58Alphabet::Bitcoin, Hash::HasherGroestl512d);
}

} // namespace TW::Groestlcoin
