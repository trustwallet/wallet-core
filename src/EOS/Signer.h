// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Prefixes.h"
#include "Transaction.h"
#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/EOS.pb.h"

#include <stdexcept>

namespace TW::EOS {

/// Helper class that performs EOS transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

  public:
    const Data chainID;

    /// Initializes a signer with a chain identifier.
    Signer(const Data& chainID) : chainID(chainID) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const;

    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

    /// Serialize the transaction.
    Data serializeTx(const Transaction& transaction) const noexcept;
    
    static int isCanonical(uint8_t by, uint8_t sig[64]);
    
    Transaction buildTx(const Proto::SigningInput& input) const;
    Data buildUnsignedTx(const Proto::SigningInput& input) noexcept;
    std::string buildSignedTx(const Proto::SigningInput& input, const Data& signature) noexcept;
};

} // namespace TW::EOS
