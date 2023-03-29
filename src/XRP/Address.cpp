// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Base58.h"
#include <TrezorCrypto/ecdsa.h>

namespace TW::Ripple {

bool Address::isValid(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string, Rust::Base58Alphabet::Ripple);
    if (decoded.size() != Address::size) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    const auto decoded = Base58::decodeCheck(string, Rust::Base58Alphabet::Ripple);
    if (decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    /// see type prefix: https://developers.ripple.com/base58-encodings.html
    bytes[0] = 0x00;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
}

std::string Address::string() const {
    return Base58::encodeCheck(bytes, Rust::Base58Alphabet::Ripple);
}

} // namespace TW::Ripple
