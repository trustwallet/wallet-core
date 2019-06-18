// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../uint256.h"
#include "../proto/Nebulas.pb.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

namespace TW::Nebulas {

/// Helper class that performs Nebulas transaction signing.
class Signer {
  public:
    uint256_t chainID;
    
    static const char* TxPayloadBinaryType;
    static const char* TxPayloadDeployType;
    static const char* TxPayloadCallType;

    /// Initializes a signer with a chain identifier.
    explicit Signer(uint256_t chainID) : chainID(std::move(chainID)) {}

    /// Signs the transaction by a SigningInput object.
    Proto::SigningOutput sign(Proto::SigningInput& input) const noexcept;

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;

  protected:
    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;
};

} // namespace TW::Nebulas

/// Wrapper for C interface.
struct TWNebulasSigner {
    TW::Nebulas::Signer impl;
};
