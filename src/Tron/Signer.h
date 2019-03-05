// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PrivateKeySecp256k1.h"
#include "../proto/Tron.pb.h"

namespace TW {
namespace Tron {

/// Helper class that performs Tron transaction signing.
class Signer {
public:
    Signer() = delete;

    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
};

}} // namespace

/// Wrapper for C interface.
struct TWTronSigner {
    TW::Tron::Signer impl;
};
