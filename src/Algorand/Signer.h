// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AssetTransfer.h"
#include "OptInAssetTransaction.h"
#include "Transfer.h"

#include "../Data.h"
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
};

} // namespace TW::Algorand
