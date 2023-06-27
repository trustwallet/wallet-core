// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include "../proto/Binance.pb.h"

#include <TrustWalletCore/TWCoinType.h>
#include <cstdint>
#include <utility>

namespace TW::Binance {

/// Helper class that performs Binance transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input, const std::string& chainHrp) noexcept;
    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key, const std::string& chainHrp);
  public:
    Proto::SigningInput input;
    std::string chainHrp;

    /// Initializes a transaction signer.
    explicit Signer(Proto::SigningInput input, std::string chainHrp):
        input(std::move(input)),
        chainHrp(std::move(chainHrp)) {
    }

    /// Initializes a transaction signer.
    explicit Signer(Proto::SigningInput input, TWCoinType coin = TWCoinTypeBinance);

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

    TW::Data preImageHash() const;
    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) const;
    std::string signaturePreimage() const;

  private:
    TW::Data encodeTransaction(const TW::Data& signature) const;
    TW::Data encodeOrder() const;
    TW::Data encodeSignature(const TW::Data& signature) const;
    TW::Data encodeSignature(const TW::Data& signature, const PublicKey& publicKey) const;
    TW::Data aminoWrap(const std::string& raw, const TW::Data& typePrefix,
                       bool isPrefixLength) const;
};

} // namespace TW::Binance
