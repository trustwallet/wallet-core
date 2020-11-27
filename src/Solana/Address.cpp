// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Transaction.h"
#include "../Base58.h"
#include "../Base58Address.h"
#include "../Hash.h"

#include "TrezorCrypto/ed25519-donna.h"

#include <cassert>

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

/*
 * Check if given address is on ed25519 curve,
 * Based on solana code, create_program_address()
 * if curve25519_dalek::edwards::CompressedEdwardsY::from_slice(hash.as_ref())
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L135
 * https://github.com/solana-labs/solana/blob/master/sdk/program/src/pubkey.rs#L153
 */
bool Address::isValidOnCurve(const Data& data) {
    if (data.size() != 32) {
        return false;
    }
    assert(data.size() == 32);
    ge25519 r;
    int res = ge25519_unpack_negative_vartime(&r, data.data());
    return res == 0;
}
