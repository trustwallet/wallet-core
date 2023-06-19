// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Staking.h"
#include "Transaction.h"
#include "Data.h"
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
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

  private:
    static Proto::SigningOutput
    signTransaction(const Proto::SigningInput &input);

    template <typename T>
    static Proto::SigningOutput signStaking(const Proto::SigningInput &input, function<T(const Proto::SigningInput &input)> func);

    template <typename T>
    static uint8_t getEnum() noexcept;

    template <typename T>
    static Staking<T> buildUnsignedStakingTransaction(const Proto::SigningInput &input, function<T(const Proto::SigningInput &input)> func);
    
    template <typename T>
    Proto::SigningOutput buildStakingSigningOutput(const Proto::SigningInput &input, const Data &signature, function<T(const Proto::SigningInput &input)> func);

    static Transaction buildUnsignedTransaction(const Proto::SigningInput &input);
    static CreateValidator buildUnsignedCreateValidator(const Proto::SigningInput &input);
    static EditValidator buildUnsignedEditValidator(const Proto::SigningInput &input);
    static Delegate buildUnsignedDelegate(const Proto::SigningInput &input);
    static Undelegate buildUnsignedUndelegate(const Proto::SigningInput &input);
    static CollectRewards buildUnsignedCollectRewards(const Proto::SigningInput &input);

  public:
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    template <typename T>
    static Proto::SigningOutput prepareOutput(const Data &encoded, const T &transaction) noexcept;

    /// Signs the given transaction.
    template <typename T>
    void sign(const PrivateKey &privateKey, const Data &hash, T &transaction) const noexcept;

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// \returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t>
    sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash) noexcept;

    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// \returns the r, s, and v values of the transaction signature
    static std::tuple<uint256_t, uint256_t, uint256_t> values(const uint256_t &chainID,
                                                              const Data &signature) noexcept;

    std::string txnAsRLPHex(Transaction &transaction) const noexcept;

    template <typename Directive>
    std::string txnAsRLPHex(Staking<Directive> &transaction) const noexcept;

    Data buildUnsignedTxBytes(const Proto::SigningInput &input);
    Proto::SigningOutput buildSigningOutput(const Proto::SigningInput &input, const Data &signature);

  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction &transaction) const noexcept;
    /// Computes the staking transaction hash.
    template <typename Directive>
    Data hash(const Staking<Directive> &transaction) const noexcept;
    // Plain rlp encoding of transaction before hashing
    Data rlpNoHash(const Transaction &transaction, const bool) const noexcept;
    // Plain rlp encoding of staking transaction before hashing
    template <typename Directive>
    Data rlpNoHash(const Staking<Directive> &transaction, const bool) const noexcept;

    Data rlpNoHashDirective(const Staking<CreateValidator> &transaction) const noexcept;
    Data rlpNoHashDirective(const Staking<EditValidator> &transaction) const noexcept;
    Data rlpNoHashDirective(const Staking<Delegate> &transaction) const noexcept;
    Data rlpNoHashDirective(const Staking<Undelegate> &transaction) const noexcept;
    Data rlpNoHashDirective(const Staking<CollectRewards> &transaction) const noexcept;
};

} // namespace TW::Harmony
