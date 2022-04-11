// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "Transaction.h"
#include "../Data.h"
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

    Signer() = default;
    /// Initializes a signer with a chain identifier which could be `mainnet`, `testnet` or
    /// `privatenet`
    explicit Signer(std::string chainID) : chainID(std::move(chainID)) {}

    /// Signs the given transaction
    Data sign(const PrivateKey& privateKey, const Transaction& transaction) noexcept;

  private:
    Data encode(const Transaction& transaction) noexcept;
};

} // namespace TW::Theta
