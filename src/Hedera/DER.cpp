// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "DER.h"
#include "PublicKey.h"
#include "HexCoding.h"

namespace TW::Hedera {

bool hasDerPrefix(const std::string& input) noexcept {
    // Anchored at the start: accepting the prefix anywhere would validate strings that
    // carry leading junk and then fail to round-trip through Address::string().
    const std::string prefix(gHederaDerPrefixPublic);
    if (!input.starts_with(prefix)) {
        return false;
    }
    return PublicKey::isValid(parse_hex(input.substr(prefix.size())), TWPublicKeyTypeED25519);
}

} // namespace TW::Hedera
