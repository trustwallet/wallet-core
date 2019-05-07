// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::Semux {

/// Helper class that performs Semux transaction signing.
class Signer {
  public:
    /// Signs the given transaction.
    static void sign(const PrivateKey &privateKey, Transaction &transaction) noexcept;
};

} // namespace TW::Semux

/// Wrapper for C interface.
struct TWSemuxSigner {
    TW::Semux::Signer impl;
};
