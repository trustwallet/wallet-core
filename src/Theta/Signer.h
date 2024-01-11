// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>

#include "Transaction.h"
#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Theta.pb.h"

namespace TW::Theta {

/// Helper class that performs Theta transaction signing
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

  public:
    std::string chainID;
    Proto::SigningInput input;
    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : chainID(input.chain_id()), input(input) {}
    /// Initializes a signer with a chain identifier which could be `mainnet`, `testnet` or
    /// `privatenet`
    explicit Signer(std::string chainID) : chainID(std::move(chainID)) {}

    /// Signs the given transaction
    Data sign(const PrivateKey& privateKey, const Transaction& transaction) noexcept;
    Data encode(const Transaction& transaction) const;
    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) const;
    Data signaturePreimage() const;
    Transaction buildTransaction() const;
};

} // namespace TW::Theta
