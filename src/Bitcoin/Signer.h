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

    /// Collects pre-image hashes to be signed
    static Proto::PreSigningOutput preImageHashes(const Proto::SigningInput& input) noexcept;

    /// Compiles a transaction with the given signatures and public keys.
    static Proto::SigningOutput compile(const Proto::SigningInput& input,
                                        const std::vector<Data>& signatures,
                                        const std::vector<PublicKey>& publicKeys) noexcept;

    /// Plans a transaction via BitcoinV2 protocol (utxo selection, fee estimation).
    static Proto::TransactionPlan planAsV2(const Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction via BitcoinV2 protocol.
    static Proto::SigningOutput signAsV2(const Proto::SigningInput& input) noexcept;

    /// Collects pre-image hashes to be signed via BitcoinV2 protocol.
    static Proto::PreSigningOutput preImageHashesAsV2(const Proto::SigningInput& input) noexcept;

    /// Compiles a transaction with the given signatures and public keys via BitcoinV2 protocol.
    static Proto::SigningOutput compileAsV2(const Proto::SigningInput& input,
                                            const std::vector<Data>& signatures,
                                            const std::vector<PublicKey>& publicKeys) noexcept;
};

} // namespace TW::Bitcoin
