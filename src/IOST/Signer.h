// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/IOST.pb.h"

namespace TW::IOST {

/// Helper class that performs IOST transaction signing.
class Signer {
  public:
    /// Hide default constructor
    //    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::IOST

/// Wrapper for C interface.
struct TWIOSTSigner {
    TW::IOST::Signer impl;
};
