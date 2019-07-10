#pragma once

#include <stdexcept>

#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::Bravo {

/// Helper class that performs Bravo transaction signing.
class Signer {
public:
    const Data chainID;

    /// Initializes a signer with a chain identifier.
    Signer(const Data& chainID) : chainID(chainID) {}

    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction, int(*canonicalChecker)(uint8_t by, uint8_t sig[64]) = is_canonical) const;
    
    /// Computes the transaction hash.
    Data hash(const Transaction& transaction) const noexcept;

	static int is_canonical(uint8_t by, uint8_t sig[64]);
};

} // namespace