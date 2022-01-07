// Copyright © 2017-2019 Trust Wallet.
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
    uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs a Proto::SigningInput transaction
    Proto::SigningOutput sign(const Proto::SigningInput &input) const noexcept;

    /// Signs the given transaction.
    Signature sign(const PrivateKey& privateKey, const std::shared_ptr<TransactionBase> transaction) const noexcept;

    Data serialize(const std::shared_ptr<TransactionBase> transaction) const noexcept;

    /// Computes the transaction hash.
    Data hash(const std::shared_ptr<TransactionBase> transaction) const noexcept;

    /// build Transaction from signing input
    static std::shared_ptr<TransactionBase> build(const Proto::SigningInput &input);

    /// Signs a hash with the given private key for the given chain identifier.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static Signature sign(const uint256_t &chainID, const PrivateKey &privateKey, const Data &hash, bool includeEip155) noexcept;

    /// R, S, and V values for the given chain identifier and signature.
    ///
    /// @returns the r, s, and v values of the transaction signature
    static Signature values(const uint256_t &chainID, const Data &signature, bool includeEip155) noexcept;
};

} // namespace TW::Ethereum

/// Wrapper for C interface.
struct TWEthereumSigner {
    TW::Ethereum::Signer impl;
};
