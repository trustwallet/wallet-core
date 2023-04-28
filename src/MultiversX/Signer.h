// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
