// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Base58.h"
#include "../Base58Address.h"

using namespace TW;
using namespace TW::Solana;

bool Address::isValid(const std::string& string) {
    const auto data = Base58::bitcoin.decode(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    const auto data = Base58::bitcoin.decode(string);
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

std::string Address::string() const {
    return Base58::bitcoin.encode(bytes);
}

Data Address::vector() const {
    Data vec(std::begin(bytes), std::end(bytes));
    return vec;
}

Address addressFromValidatorSeed(Address& fromAddress, Address& validatorAddress,
                                 Address& programId) {
    Data extended = fromAddress.vector();
    std::string seed = validatorAddress.string();
    Data vecSeed(seed.begin(), seed.end());
    vecSeed.resize(32);
    Data additional = programId.vector();
    extended.insert(extended.end(), vecSeed.begin(), vecSeed.end());
    extended.insert(extended.end(), additional.begin(), additional.end());
    Data hash = TW::Hash::sha256(extended);
    return Address(PublicKey(hash, TWPublicKeyTypeED25519));
}
