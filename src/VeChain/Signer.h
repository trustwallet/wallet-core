// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"

#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::VeChain {

/// Helper class that performs VeChain transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    static Data buildUnsignedTx(const Proto::SigningInput& input) noexcept;

    static Data buildSignedTx(const Proto::SigningInput& input, const Data& signature) noexcept;

    /// Signs the given transaction.
    static Data sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;
};

} // namespace TW::VeChain
