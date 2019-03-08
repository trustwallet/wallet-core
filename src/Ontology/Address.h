// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "PublicKey.h"

#include <array>
#include <vector>

namespace TW {
namespace Ontology {

class Address {

private:

    std::vector<uint8_t> toScriptHash(std::vector<uint8_t> &data);

public:

    static const size_t size = 20;
    static const uint8_t version = 0x17;

    std::array<uint8_t, size> zero;

    explicit Address(const PublicKey &publicKey);

    explicit Address(const std::string &b58Address);

    explicit Address(const std::vector<uint8_t> &data);

    static bool isValid(const std::vector<uint8_t> &data) {
        return data.size() == size;
    }

    static bool isValid(const std::string &string);

    std::string hexString() const;

    std::string b58String() const;
};
}
}
