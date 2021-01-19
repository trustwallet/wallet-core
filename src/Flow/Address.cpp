// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "HexCoding.h"

#include <cassert>

using namespace TW::Flow;

bool Address::isValid(const std::string& string) {
    // TODO: Finalize implementation
    return false;
}

Address::Address(const std::string& string) {
    // TODO: Finalize implementation

    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const std::string& accountName, const PublicKey& publicKey) : accountName(accountName) {
    assert(publicKey.type == TWPublicKeyTypeSECP256k1);
    std::string key = hex(publicKey.bytes); // TODO conversion
    keys.push_back(KeyStruct(key));
}

std::string Address::string() const {
    return accountName;
}
