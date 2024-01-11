// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PrivateKey.h"
#include "../proto/MultiversX.pb.h"

namespace TW::MultiversX {

/// Helper class that performs MultiversX transaction signing.
class Signer {
public:
    /// Hide default constructor
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
    
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

    static Data buildUnsignedTxBytes(const Proto::SigningInput &input);
    static Proto::SigningOutput buildSigningOutput(const Proto::SigningInput &input, const Data &signature);
};

} // namespace TW::MultiversX
