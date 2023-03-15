// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
