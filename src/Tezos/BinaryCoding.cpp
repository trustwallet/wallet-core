// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../Data.h"
#include "../HexCoding.h"
#include "../PublicKey.h"
#include "../PrivateKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <string>

using namespace TW;

std::string base58ToHex(const std::string& string, size_t prefixLength, uint8_t* prefix) {
    const auto decoded = Base58::bitcoin.decodeCheck(string);
    if (decoded.size() < prefixLength) {
        return "";
    }
    return TW::hex(decoded.data() + prefixLength, decoded.data() + decoded.size());
}

PublicKey parsePublicKey(const std::string& publicKey) {
    const auto decoded = Base58::bitcoin.decodeCheck(publicKey);

    std::array<byte, 4> prefix = {13, 15, 37, 217};
    auto pk = Data();

    if (decoded.size() != 32 + prefix.size()) {
        throw std::invalid_argument("Invalid Public Key");
    }
    append(pk, Data(decoded.begin() + prefix.size(), decoded.end()));

    return PublicKey(pk, TWPublicKeyTypeED25519);
}

PrivateKey parsePrivateKey(const std::string& privateKey) {
    const auto decoded = Base58::bitcoin.decodeCheck(privateKey);
    auto pk = Data();
    auto prefix_size = 4;

    if (decoded.size() != 32 + prefix_size) {
        throw std::invalid_argument("Invalid Public Key");
    }
    append(pk, Data(decoded.begin() + prefix_size, decoded.end()));
    return PrivateKey(pk);
}