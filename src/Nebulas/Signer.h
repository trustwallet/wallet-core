// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Nebulas.pb.h"
#include "../uint256.h"

namespace TW::Nebulas {

/// Helper class that performs Nebulas transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

  public:
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

    /// Computes hash.
    Data hash(const Data& preImage) const noexcept;

    Transaction buildTransaction(const Proto::SigningInput& input) const noexcept;

    /// Get transaction data.
    Data getPreImage(const Transaction& transaction) const noexcept;
};

} // namespace TW::Nebulas
