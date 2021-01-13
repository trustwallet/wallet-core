// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"

using namespace TW::Avalanche;

bool Address::isValid(const std::string& string) {
    // TODO: Finalize implementation
    // split into before and after - 
    std::string delimiter = "-";
    auto chainID = string.substr(string.find("-"));
    // compare before-hyphen with 'X' stringcompare (make it smarter later)
    if (chainID != "X") {
        // implementation is currently X-chain only
        return false;
    }
    // decode after-hyphen with bech32
    auto afterHyphen = string.substr(string.find("-") + 1);
    auto decoded = Bech32::decode(afterHyphen);
    if (decoded.second.size() == 0) {
        // decode failure
        return false;
    }
    // check hrp against 'fuji' (make it smarter later)
    if (decoded.first != "fuji") {
        // implementation is currently fuji only
        return false;
    }
    if (!PublicKey::isValid(decoded.second, TWPublicKeyTypeSECP256k1)) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    auto afterHyphen = string.substr(string.find("-") + 1);
    auto decoded = Bech32::decode(afterHyphen);
    hrp = decoded.first;
    std::copy(decoded.second.begin(), decoded.second.begin() + PublicKey::secp256k1Size, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Invalid public key type");
    }
    std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.data());
}

std::string Address::string() const {
    Data data;
    data.resize(PublicKey::secp256k1Size);
    std::copy(bytes.begin(), bytes.end(), data.data());
    std::string encoded = Bech32::encode(hrp, data);
    // TODO make X- smarter later
    return "X-" + encoded;
}