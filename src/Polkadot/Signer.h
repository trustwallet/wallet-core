// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
