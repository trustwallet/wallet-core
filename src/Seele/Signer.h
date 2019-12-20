// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "RLP.h"
#include "Transaction.h"
#include "../Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../uint256.h"
#include "../proto/Seele.pb.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>
#include <stdint.h>

namespace TW::Seele {

/// Helper class that performs Seele transaction signing.
class Signer {
public:
    Proto::SigningInput input;

    Proto::Transaction transaction;

    /// Initializes a transaction signer.
    Signer(Proto::SigningInput&& input);

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    Data sign() const;

    /// Builds the signed transaction.
    ///
    /// \returns the signed transaction.
    Proto::SigningOutput build() const;



private:
    std::string signaturePreimage() const;
    nlohmann::json buildTransactionJSON(const Data& signature) const;
    std::string buildTransaction() const;

protected:
    /// Computes the transaction hash.
    Data hash(const Proto::SignTransaction& transaction) const noexcept;
};



} // namespace

/// Wrapper for C interface.
struct TWSeeleSigner {
    TW::Seele::Signer impl;
};