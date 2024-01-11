// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "AssetTransfer.h"
#include "proto/Common.pb.h"
#include "OptInAssetTransaction.h"
#include "Transfer.h"

#include "Data.h"
#include "../PrivateKey.h"

namespace TW::Algorand {

/// Helper class that performs Algorand transaction signing.
class Signer {
  public:
    Signer() = delete;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

    /// Signs a json Proto::SigningInput with private key
    static std::string signJSON(const std::string& json, const Data& key);

    /// Signs the given transaction.
    static Data sign(const PrivateKey& privateKey, const BaseTransaction& transaction) noexcept;

    /// Get transaction data to be signed
    static TW::Data signaturePreimage(const Proto::SigningInput& input) noexcept;
    static Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept;

  private:
    static TW::Data preImage(const TW::PublicKey& pubKey, const Proto::SigningInput& input) noexcept;
    static Proto::SigningOutput encodeTransaction(const Data& signature, const TW::PublicKey& pubKey, const Proto::SigningInput& input) noexcept;
};

} // namespace TW::Algorand
