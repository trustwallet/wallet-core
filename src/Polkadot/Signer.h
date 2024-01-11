// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/Polkadot.pb.h"

namespace TW::Polkadot {

/// Helper class that performs Polkadot transaction signing.
class Signer {
public:
    /// Hide default constructor
    explicit Signer();

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    static Data signaturePreImage(const Proto::SigningInput &input);
    static Data encodeTransaction(const Proto::SigningInput &input, const Data &publicKey, const Data &signature);
    static Data hash(const Data &payload);
};

} // namespace TW::Polkadot
