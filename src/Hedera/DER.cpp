// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
