// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../proto/NULS.pb.h"

#include <stdint.h>
#include "Data.h"
#include "../uint256.h"

namespace TW::NULS {

/// Helper class that performs NULS transaction signing.
class Signer {
  public:
    Proto::Transaction tx;

    Proto::SigningInput input;

    /// Initializes a transaction signer.
    Signer(Proto::SigningInput& input);

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    Data sign() const;
};

} // namespace TW::NULS

/// Wrapper for C interface.
struct TWNULSSigner {
    TW::NULS::Signer impl;
};
