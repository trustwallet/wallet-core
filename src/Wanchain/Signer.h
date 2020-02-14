// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Ethereum/RLP.h"
#include "../Ethereum/Transaction.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Ethereum.pb.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Wanchain {

/// Helper class that performs Wanchain transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Ethereum::Proto::SigningOutput sign(const Ethereum::Proto::SigningInput& input) noexcept;
  public:
    boost::multiprecision::uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    Signer(boost::multiprecision::uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs the given transaction.
    void sign(const PrivateKey &privateKey, Ethereum::Transaction &transaction) const noexcept;

    /// Encodes a transaction.
    Data encode(const Ethereum::Transaction &transaction) const noexcept;

  protected:
    /// Computes the transaction hash.
    Data hash(const Ethereum::Transaction &transaction) const noexcept;
};

} // namespace TW::Wanchain
