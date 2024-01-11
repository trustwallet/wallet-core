// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Aion.pb.h"

#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Aion {

/// Helper class that performs Aion transaction signing.
class Signer {
  public:
    Signer() = delete;
    
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    static void sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;

    /// Get transaction data to be signed
    static TW::Data signaturePreimage(const Proto::SigningInput& input) noexcept;
    static Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept;

  private:
    /// Builds an Aion transaction from the given `Proto::SigningInput`.
    static Transaction buildTransaction(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::Aion
