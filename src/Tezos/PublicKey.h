#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW {
namespace Tezos {

class PublicKey {
public:
    std::string public_key;
    /// Initializes a Tezos address with a public_key
    PublicKey(const std::string& string);
    /// Returns a string representation of the address.
    std::string forge() const;
};

}} // namespace

/// Wrapper for C interface.
struct TWTezosPublicKey {
    TW::Tezos::PublicKey impl;
};
