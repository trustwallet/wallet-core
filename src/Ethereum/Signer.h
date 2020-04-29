// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "RLP.h"
#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Ethereum.pb.h"
#include "../uint256.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Ethereum {

/// Helper class that performs Ethereum transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

  public:
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs the given transaction.
    void sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept;

  public:
    /// build Transaction from signing input
    static Transaction build(const Proto::SigningInput &input);

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t>
    sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data& hash) noexcept;

    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t> values(const uint256_t &chainID,
                                                              const Data& signature) noexcept;

  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction &transaction) const noexcept;
};

} // namespace TW::Ethereum

/// Wrapper for C interface.
struct TWEthereumSigner {
    TW::Ethereum::Signer impl;
};
