// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/NEAR.pb.h"

namespace TW::NEAR {

/// Helper class that performs NEAR transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

} // namespace

/// Wrapper for C interface.
struct TWNEARSigner {
    TW::NEAR::Signer impl;
};
