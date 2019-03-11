// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

using namespace TW::NEO;

bool Address::isValid(const std::string& string) {
    return true;
}

Address::Address(const std::string& string) {
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(isValid(data));
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto publicKeyData = publicKey.bytes;
    std::copy(publicKeyData.begin() + 1, publicKeyData.begin() + 33, bytes.data());
}

std::string Address::string() const {
    return "";
}
