// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKeySecp256k1.h"
#include "Transaction.h"

namespace TW {
namespace Ripple {

/// Helper class that performs Ripple transaction signing.
class Signer {
public:
    /// Signs the given transaction.
    void sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept;
};

}} // namespace

/// Wrapper for C interface.
struct TWRippleSigner {
    TW::Ripple::Signer impl;
};
