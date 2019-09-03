
// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "AddressChecksum.h"

#include "../Bech32.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHRP.h>

using namespace TW::Harmony;

bool Address::isValid(const std::string &addr) {
    if (addr.size() != 42 || addr[0] != '0' || addr[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(addr);
    return Address::isValid(data);
}

bool Address::isValid(const Data &data) {
    // Easiest low threshold of checking validity
    return data.size() == size;
}

Address::Address(const std::string &string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data &data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey &publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Harmony::Address needs an extended SECP256k1 public key.");
    }
    const auto data = publicKey.hash(
        {}, static_cast<Data (*)(const byte *, const byte *)>(Hash::keccak256), true);
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string() const {
    return Harmony::checksumed(static_cast<const TW::Harmony::Address>(*this),
                               Harmony::Checksum::eip55);
}
