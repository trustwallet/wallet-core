// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "Data.h"
#include "../HexCoding.h"
#include "../PublicKey.h"
#include "../PrivateKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <string>

namespace TW::Tezos {

std::string base58ToHex(const std::string& string, size_t prefixLength) {
    const auto decoded = Base58::decodeCheck(string);
    if (decoded.size() < prefixLength) {
        return "";
    }
    Data v(decoded.data() + prefixLength, decoded.data() + decoded.size());
    return TW::hex(v);
}

PublicKey parsePublicKey(const std::string& publicKey) {
    const auto decoded = Base58::decodeCheck(publicKey);

    std::array<byte, 4> prefix;
    enum TWPublicKeyType type;
    std::array<byte, 4> ed25519Prefix = {13, 15, 37, 217};
    std::array<byte, 4> secp256k1Prefix = {3, 254, 226, 86};

    if (std::equal(std::begin(ed25519Prefix), std::end(ed25519Prefix), std::begin(decoded))) {
        prefix = ed25519Prefix;
        type = TWPublicKeyTypeED25519;
    } else if (std::equal(std::begin(secp256k1Prefix), std::end(secp256k1Prefix), std::begin(decoded))) {
        prefix = secp256k1Prefix;
        type = TWPublicKeyTypeSECP256k1;
    } else {
        throw std::invalid_argument("Unsupported Public Key Type");
    }
    auto pk = Data();
    if (type == TWPublicKeyTypeED25519 && decoded.size() != 32 + prefix.size()) {
        throw std::invalid_argument("Invalid Public Key");
    }
    if (type == TWPublicKeyTypeSECP256k1 && decoded.size() != 33 + prefix.size()) {
        throw std::invalid_argument("Invalid Public Key");
    }
    append(pk, Data(decoded.begin() + prefix.size(), decoded.end()));

    return PublicKey(pk, type);
}

PrivateKey parsePrivateKey(const std::string& privateKey) {
    const auto decoded = Base58::decodeCheck(privateKey);
    auto pk = Data();
    auto prefix_size = 4ul;

    if (decoded.size() != 32 + prefix_size) {
        throw std::invalid_argument("Invalid Public Key");
    }
    append(pk, Data(decoded.begin() + prefix_size, decoded.end()));
    return PrivateKey(pk);
}

} // namespace TW::Tezos
