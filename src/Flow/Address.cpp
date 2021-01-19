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
    if (string.length() != 18 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    // try to parse
    Data accountId = parse_hex(string);
    if (accountId.size() != 8) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    accountId = string;
}

/*
Address::Address(const PublicKey& publicKey) : accountId(accountId) {
    assert(publicKey.type == TWPublicKeyTypeSECP256k1);
    // TODO
}
*/

std::string Address::string() const {
    return accountId;
}
