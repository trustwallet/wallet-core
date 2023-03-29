// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../proto/Cosmos.pb.h"
#include <TrustWalletCore/TWCoinType.h>

namespace TW::THORChain {

/// Helper class that performs THORChain transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Cosmos::Proto::SigningOutput sign(Cosmos::Proto::SigningInput& input) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);
};

} // namespace TW::THORChain
