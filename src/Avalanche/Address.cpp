// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <iostream>

using namespace TW::Avalanche;

bool Address::isValid(const std::string& string) {
    std::cout << "testing " << string << std::endl;
    // split into before and after - 
    auto hyphenPos = string.find("-");
    if (hyphenPos == std::string::npos) {
        std::cout << "hyphenpos" << std::endl;
        return false;
    }

    auto chainID = string.substr(hyphenPos - 1, 1);
    // compare before-hyphen with 'X' stringcompare (make it smarter later)
    if (chainID != "X") {
        std::cout << "chainID was " << chainID << std::endl;
        // implementation is currently X-chain only
        return false;
    }
    // decode after-hyphen with bech32
    auto afterHyphen = string.substr(hyphenPos + 1);
    auto decoded = Bech32::decode(afterHyphen);
    if (decoded.second.size() != 32) {
        // decode failure
        return false;
    }
    // check hrp against 'avax' (make it smarter later)
    if (decoded.first != "avax") {
        // implementation is currently avax only
        std::cout << "decoded.first wasn't avax somehow" << std::endl;
        return false;
    }
    // an address is a hash of the public key, so we cannot derive the public key from string for PubKey::isValid
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    auto hyphenPos = string.find("-");
    auto decoded = Bech32::decode(string.substr(hyphenPos + 1));
    bytes.resize(addressSize);
    std::copy_n(decoded.second.begin(), addressSize, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Invalid public key type");
    }
    std::cout << "no segfault yet, just validated type" << std::endl;
    auto result = Hash::ripemd(Hash::sha256(publicKey.bytes));
    std::cout << "no segfault yet, just double hasehd. result was" << hexEncoded(result) << " with len " << result.size() << std::endl;
    bytes.resize(addressSize);
    std::copy_n(result.begin(), addressSize, bytes.begin());
}

std::string Address::string() const {
    return "X-" + Bech32::encode("avax", bytes);
}