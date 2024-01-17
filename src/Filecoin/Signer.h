// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"

#include "Data.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../proto/Filecoin.pb.h"

namespace TW::Filecoin {

/// Helper class that performs Filecoin transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

    /// Signs the given transaction.
    static Data sign(const PrivateKey& privateKey, Transaction& transaction) noexcept;

    /// Get transaction data to be signed
    static TW::Data signaturePreimage(const Proto::SigningInput& input) noexcept;

    /// build transaction with signature
    static Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept;
  
  private:
    /// Get transaction data for secp256k1 to be signed
    static Transaction buildTx(const PublicKey& publicKey, const Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction.
    static Proto::SigningOutput signSecp256k1(const Proto::SigningInput& input);

    /// Signs a Proto::SigningInput transaction.
    static Proto::SigningOutput signDelegated(const Proto::SigningInput& input);
};

} // namespace TW::Filecoin
