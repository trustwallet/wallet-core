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

    std::array<uint8_t, size> data;

    explicit Address(const PublicKey &publicKey);

    explicit Address(const std::string &b58Address);

    explicit Address(const std::vector<uint8_t> &bytes);

    static bool isValid(const std::string &string) noexcept;

    std::string string() const;
};

static inline bool operator==(const Address &lhs, const Address &rhs) {
    return lhs.data == rhs.data;
}

}
} // namespace

/// Wrapper for C interface.
struct TWOntologyAddress {
    TW::Ontology::Address impl;
};
