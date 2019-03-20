// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../proto/NULS.pb.h"

#include <stdint.h>
#include <vector>

namespace TW {
namespace NULS {

/// Helper class that performs NULS transaction signing.
class Signer {
public:
    Proto::Transaction tx;

    Signer(Proto::Transaction tx)
            :tx(tx) { };

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    std::vector<uint8_t> sign(uint64_t timestamp) const;

    /**
     * Calculate transaction need fee
     * @param inputCount  how much input records
     * @param outputCount how much outout records
     * @param remarkSize  UTF-8 encode remark string length. Empty is 0
     * @return needed fee
     */
    static uint64_t getFee(uint32_t inputCount, uint32_t outputCount, uint32_t remarkSize);
};

}
} // namespace

/// Wrapper for C interface.
struct TWNULSSigner {
  TW::NULS::Signer impl;
};
