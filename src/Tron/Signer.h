// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Tron.pb.h"

namespace TW::Tron {

/// Helper class that performs Tron transaction signing.
class Signer {
  public:
    Proto::SigningInput input;
    Signer() = delete;
    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}
    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    Proto::SigningOutput compile(const Data& signature) const;
    Data signaturePreimage() const;
};

} // namespace TW::Tron
