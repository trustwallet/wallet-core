// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Binance.pb.h"
#include "Data.h"

#include <cstdint>

namespace TW::Binance {

/// Helper class that performs Binance transaction signing.
class Signer {
  public:
    Proto::SigningInput input;

    /// Initializes a transaction signer.
    explicit Signer(Proto::SigningInput&& input) : input(input) {}

    /// Builds a signed transaction.
    ///
    /// \returns the signed transaction data or an empty vector if there is an
    /// error.
    TW::Data build() const;

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an
    /// error.
    TW::Data sign() const;

  private:
    std::string signaturePreimage() const;
    TW::Data encodeTransaction(const TW::Data& signature) const;
    TW::Data encodeOrder() const;
    TW::Data encodeSignature(const TW::Data& signature) const;
    TW::Data aminoWrap(const std::string& raw, const TW::Data& typePrefix,
                                   bool isPrefixLength) const;
};

} // namespace TW::Binance

/// Wrapper for C interface.
struct TWBinanceSigner {
    TW::Binance::Signer impl;
};
