// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <stdexcept>

#include "Transaction.h"
#include "Prefixes.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::EOS {

/// Helper class that performs EOS transaction signing.
class Signer {
public:
    const Data chainID;

    /// Initializes a signer with a chain identifier.
    Signer(const Data& chainID) : chainID(chainID) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const;
    
    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

    static int is_canonical(uint8_t by, uint8_t sig[64]);
};

} // namespace TW::EOS