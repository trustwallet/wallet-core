// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "DER.h"
#include "PublicKey.h"
#include "HexCoding.h"

namespace TW::Hedera {

bool hasDerPrefix(const std::string& input) noexcept {
    if (std::size_t pos = input.find(gHederaDerPrefixPublic); pos != std::string::npos) {
        return PublicKey::isValid(parse_hex(input.substr(pos + std::string(gHederaDerPrefixPublic).size())), TWPublicKeyTypeED25519);
    }
    return false;
}

} // namespace TW::Hedera
