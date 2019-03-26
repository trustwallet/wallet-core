// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Binance.pb.h"

#include <stdint.h>
#include <vector>

namespace TW {
namespace Binance {

/// Helper class that performs Binance transaction signing.
class Signer {
public:
    Proto::SigningInput input;

    /// Initializes a transaction signer.
    explicit Signer(Proto::SigningInput&& input) : input(input) {}

    /// Builds a signed transaction.
    ///
    /// \returns the signed transaction data or an empty vector if there is an error.
    std::vector<uint8_t> build() const;

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    std::vector<uint8_t> sign() const;

private:
    std::string signaturePreimage() const;
    std::vector<uint8_t> encodeTransaction(const std::vector<uint8_t>& signature) const;
    std::vector<uint8_t> encodeOrder() const;
    std::vector<uint8_t> encodeSignature(const std::vector<uint8_t>& signature) const;
    std::vector<uint8_t> aminoWrap(const std::string& raw, const std::vector<uint8_t>& typePrefix, bool isPrefixLength) const;
};

}} // namespace

/// Wrapper for C interface.
struct TWBinanceSigner {
    TW::Binance::Signer impl;
};
