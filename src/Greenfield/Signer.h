// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"
#include "proto/Cosmos.pb.h"
#include "proto/TransactionCompiler.pb.h"

namespace TW::Greenfield {

/// Helper class that performs Greenfield transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Cosmos::Proto::SigningOutput sign(const Cosmos::Proto::SigningInput& input);

    /// Collect pre-image hashes to be signed
    static TxCompiler::Proto::PreSigningOutput preImageHashes(const Cosmos::Proto::SigningInput& input);
};

} // namespace TW::Greenfield
