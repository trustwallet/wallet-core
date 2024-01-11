// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Zilliqa.pb.h"

namespace TW::Zilliqa {

/// Helper class that performs Zilliqa transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// Signs the given signing input
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

    /// compute preImage and decode address from signing input.
    static Data getPreImage(const Proto::SigningInput& input, Address& address) noexcept;
};

} // namespace TW::Zilliqa
