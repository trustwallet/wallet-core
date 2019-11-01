// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "StakingTransaction.h"
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
class StakingSigner {
  public:
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit StakingSigner(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs a Proto::SigningInput transaction
    static Proto::StakingTransactionOutput
    sign(const Proto::StakingTransactionInput &input) noexcept;

    /// Signs the given transaction.
    template <typename Directive>
    void sign(const PrivateKey &privateKey, StakingTransaction<Directive> &transaction) const
        noexcept;

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

    template <typename Directive>
    std::string txnAsRLPHex(StakingTransaction<Directive> &transaction) const noexcept;

    static uint256_t deriveChainID(const uint256_t v) noexcept;

  protected:
    /// Computes the staking transaction hash.
    template <typename Directive>
    Data hash(const StakingTransaction<Directive> &transaction) const noexcept;
    // Plain rlp encoding before hashing
    template <typename Directive>
    Data rlpNoHash(const StakingTransaction<Directive> &transaction, const bool) const noexcept;

    Data rlpNoHashDirective(const StakingTransaction<CreateValidator> &transaction) const noexcept;
    Data rlpNoHashDirective(const StakingTransaction<EditValidator> &transaction) const noexcept;
    Data rlpNoHashDirective(const StakingTransaction<Delegate> &transaction) const noexcept;
    Data rlpNoHashDirective(const StakingTransaction<Undelegate> &transaction) const noexcept;
    Data rlpNoHashDirective(const StakingTransaction<CollectRewards> &transaction) const noexcept;
};

} // namespace TW::Harmony

/// Wrapper for C interface.
struct TWHarmonyStakingSigner {
    TW::Harmony::StakingSigner impl;
};
