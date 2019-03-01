// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrivateKey.h"
#include "PublicKey.h"

using namespace TW::Aion;

PrivateKey::~PrivateKey() {
    std::fill(bytes.begin(), bytes.end(), 0);
}

std::vector<uint8_t> PrivateKey::getPublicKey() const {
    std::vector<uint8_t> result(bytes.end() - PublicKey::size, bytes.end());
    return result;
}
