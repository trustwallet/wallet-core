// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Program.h"
#include "Transaction.h"
#include "../Base58.h"
#include "../Base58Address.h"
#include "../Hash.h"

#include <TrezorCrypto/ed25519-donna/ed25519-donna.h>

#include <cassert>

using namespace TW;

namespace TW::Solana {

bool Address::isValid(const std::string& string) {
    const auto data = Base58::decode(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    const auto data = Base58::decode(string);
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    static_assert(PublicKey::ed25519Size == size);
    std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.data());
}

Address::Address(const Data& publicKeyData) {
    if (publicKeyData.size() != PublicKey::ed25519Size) {
        throw std::invalid_argument("Invalid public key data size");
    }
    std::copy(publicKeyData.begin(), publicKeyData.end(), bytes.data());
}

std::string Address::string() const {
    return Base58::encode(bytes);
}

Data Address::vector() const {
    return Data(begin(bytes), end(bytes));
}

Address Address::defaultTokenAddress(const Address& tokenMintAddress) {
    return TokenProgram::defaultTokenAddress(*this, tokenMintAddress);
}

} // namespace TW::Solana
