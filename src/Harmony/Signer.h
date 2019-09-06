// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Harmony.pb.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Harmony {

/// Helper class that performs Harmony transaction signing.
class Signer {
  public:
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput &input) noexcept;

    /// Signs the given transaction.
    void sign(const PrivateKey &privateKey, Transaction &transaction) const noexcept;

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t>
    sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash) noexcept;

    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t> values(const uint256_t &chainID,
                                                              const Data &signature) noexcept;

    std::string txnAsRLPHex(Transaction &transaction) const noexcept;

  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction &transaction) const noexcept;
    // Plain rlp encoding before hashing
    Data rlpNoHash(const Transaction &transaction, const bool) const noexcept;
};

} // namespace TW::Harmony

/// Wrapper for C interface.
struct TWHarmonySigner {
    TW::Harmony::Signer impl;
};
