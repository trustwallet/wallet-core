// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Ethereum/RLP.h"
#include "../Ethereum/Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW {
namespace Wanchain {

/// Helper class that performs Wanchain transaction signing.
class Signer {
public:
    boost::multiprecision::uint256_t chainID;

    /// Initializes a signer with a chain identifier.
    Signer(boost::multiprecision::uint256_t chainID) : chainID(chainID) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Ethereum::Transaction& transaction) const noexcept;

    /// Encodes a transaction.
    Data encode(const Ethereum::Transaction& transaction) const noexcept;

protected:
    /// Computes the transaction hash.
    Data hash(const Ethereum::Transaction& transaction) const noexcept;
};

}} // namespace

/// Wrapper for C interface.
struct TWWanchainSigner {
    TW::Wanchain::Signer impl;
};

