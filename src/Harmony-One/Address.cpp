
// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHRP.h>

using namespace TW::Harmony;

std::pair<bool, Data> Address::isValid(const std::string &addr) {
    if (addr.size() != 42) {
        return {false, {}};
    }
    const auto [first, second] = Bech32::decode(addr);
    if (first.size() == 0 || second.size() == 0 || first != Address::hrp) {
        return {false, {}};
    }
    return {true, second};
}

bool Address::isValid(const Data &data) {
    return data.size() == Address::size;
}

Address::Address(const std::string &addr) {
    const auto [success, payload] = isValid(addr);
    if (!success) {
        throw std::invalid_argument("address not in Harmony bech32 format");
    }
    Data as_base_32;
    Bech32::convertBits<5, 8, false>(as_base_32, payload);
    std::copy(as_base_32.begin(), as_base_32.end(), bytes.begin());
}

Address::Address(const Data &data) {
    if (!isValid(data)) {
        throw std::invalid_argument("invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("address may only be an extended SECP256k1 public key");
    }
    const auto data = publicKey.hash(
        {}, static_cast<Data (*)(const byte *, const byte *)>(Hash::keccak256), true);
    std::copy(data.end() - Address::size, data.end(), bytes.begin());
}

std::string Address::string() const {
    Data as_base_32;
    Bech32::convertBits<8, 5, false>(as_base_32, std::vector<uint8_t>(bytes.begin(), bytes.end()));
    return Bech32::encode(Address::hrp, as_base_32);
}

std::string Address::hex_dump() const {
    return hex(bytes);
}
