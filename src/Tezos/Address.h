#pragma once

#include <string>

namespace TW {
namespace Tezos {

class Address {
public:
    std::string public_key_hash;
    /// Initializes a Tezos address with a public_key hash.
    Address(const std::string& string);

    /// Initializes a Tezos address with a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string forge() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWTezosAddress {
    TW::Tezos::Address impl;
};
