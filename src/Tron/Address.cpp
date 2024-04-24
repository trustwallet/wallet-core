// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include "../Base58.h"
#include "../Hash.h"

#include <cassert>
#include <stdexcept>

namespace TW::Tron {

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string);
    if (decoded.size() != Address::size) {
        return false;
    }

    if (decoded[0] != prefix) {
        return false;
    }

    return true;
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto pkdata = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    const auto keyhash = Hash::keccak256(pkdata);
    bytes[0] = prefix;
    std::copy(keyhash.end() - size + 1, keyhash.end(), bytes.begin() + 1);
}

} // namespace TW::Tron
