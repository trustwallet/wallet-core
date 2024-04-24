// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once
#include "../proto/Bitcoin.pb.h"
#include "Data.h"
#include "CoinEntry.h"

#include <vector>
#include <optional>
#include <utility>

namespace TW::Bitcoin {

class Signer {
  public:
    Signer() = delete;

    /// Returns a transaction plan (utxo selection, fee estimation)
    static Proto::TransactionPlan plan(const Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs = {}) noexcept;

    /// Collect pre-image hashes to be signed
    static Proto::PreSigningOutput preImageHashes(const Proto::SigningInput& input) noexcept;
};

} // namespace TW::Bitcoin
