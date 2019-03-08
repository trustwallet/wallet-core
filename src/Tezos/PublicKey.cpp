// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "PublicKey.h"
#include "HexCoding.h"

using namespace TW;
using namespace TW::Tezos;

TW::Tezos::PublicKey(const std::string& pkey) {
    public_key = pkey;
}

std::string PublicKey::forge() const {
    size_t prefixLength = 4;
    uint8_t prefix[] = {13, 15, 37, 217};
    size_t capacity = 128;
    uint8_t decoded[capacity];
    int decodedLength = checkDecodeAndDropPrefix(public_key, prefixLength, prefix, decoded);

    return "00" + hex(decoded, decoded + decodedLength);
}
