// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
#pragma once

#include "Address.h"
#include "Data.h"
#include "../PrivateKey.h"
#include <proto/Nano.pb.h>

namespace TW::Nano {
/// Helper class that performs Ripple transaction signing.
class Signer {
public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

public:
    const PrivateKey privateKey;
    const PublicKey publicKey;
    const Proto::SigningInput& input;
    std::array<byte, 32> previous;
    std::array<byte, 32> link;
    const std::array<byte, 32> blockHash;

    explicit Signer(const Proto::SigningInput& input);

    /// Signs the blockHash, returns signature bytes
    std::array<byte, 64> sign() const noexcept;

    /// Builds signed transaction, incl. signature, and json format
    Proto::SigningOutput build() const;

    static Data buildUnsignedTxBytes(const Proto::SigningInput& input);
    static Proto::SigningOutput buildSigningOutput(const Proto::SigningInput& input, const Data& signature);
};

} // namespace TW::Nano
