// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "AddressChecksum.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "RLP.h"


using namespace TW::Seele;

bool Address::isValid(const std::string& string) {
    if (string.size() != 42 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Seele::Address needs an extended SECP256k1 public key.");
    }
    auto bytes1 = publicKey.bytes;
    std::vector<uint8_t> bytes2;
    for(int i=1;i<bytes1.size();i++){
        bytes2.push_back(bytes1[i]);
    }

    auto rlp =  RLP::encode(bytes2);
    auto data = Hash::keccak256(rlp);

    std::copy(data.end() - 20, data.end(), bytes.begin());
}

std::string Address::string() const {
    return checksumed(*this);
}
