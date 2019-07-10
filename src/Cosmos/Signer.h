// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include "../Data.h"
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <vector>

namespace TW::Cosmos {

/// Helper class that performs Cosmos transaction signing.
class Signer {
public:
    Proto::SigningInput input;

    /// Initializes a transaction signer.
    Signer(Proto::SigningInput&& input);

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    Data sign() const;
    std::string signInBase64() const;

    /// Builds the signed transaction.
    ///
    /// \returns the signed transaction.
    Proto::SigningOutput build() const;

private:
    std::string signaturePreimage() const;
    nlohmann::json buildTransactionJSON(const Data& signature) const;
    std::string buildTransaction() const;
};

} // namespace

/// Wrapper for C interface.
struct TWCosmosSigner {
    TW::Cosmos::Signer impl;
};
