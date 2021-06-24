// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"
#include "../PrivateKey.h"
#include "../proto/Top.pb.h"

namespace TW::Top {

/// Helper class that performs Top transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs the given transaction.
    static void sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;    
};

} // namespace TW::Top

/// Wrapper for C interface.
struct TWTopSigner {
    TW::Top::Signer impl;
};
