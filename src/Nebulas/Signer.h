// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
