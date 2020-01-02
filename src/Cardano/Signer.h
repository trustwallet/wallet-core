// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../proto/Cardano.pb.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>
#include <vector>

namespace TW::Cardano {

/// Cardano transaction signing.
class Signer {
public:
    /// Plan a transaction: compute fee, decide input UTXOs
    static Proto::TransactionPlan planTransaction(const Proto::SigningInput& input) noexcept;

    /// Build and sign a Cardano transaction, using plan from planTransaction().
    static Proto::SigningOutput sign(const Proto::SigningInput& input, const Proto::TransactionPlan& plan) noexcept;

    static const uint64_t Network_Mainnet_Protocol_Magic = 764824073;
    static const uint64_t MinimalFee = 1;
    static const uint32_t FeeLinearCoeffA = 155381;
    static constexpr double FeeLinearCoeffB = 43.946;

    // Helper methods
    /// Plan a transaction: compute fee, decide input UTXOs.  Throws on error.
    static Proto::TransactionPlan planTransactionNoFee(const Proto::SigningInput& input);
    /// Plan a transaction: given amount and fee, decide input UTXOs.  Throws on error.
    static Proto::TransactionPlan planTransactionWithFee(const Proto::SigningInput& input, uint64_t fee);
    /// Check validity of a tx plan.  Throws on error.
    static void checkPlan(const Proto::TransactionPlan& plan);
    /// Prepare unsigned Tx data (Cbor).  Returns unisgnedEncodedCborData.
    static Data prepareUnsignedTx(const Proto::SigningInput& input, const Proto::TransactionPlan& plan);
    /// Prepare signed Tx data (Cbor).
    static Proto::SigningOutput prepareSignedTx(const Proto::SigningInput& input, const Proto::TransactionPlan& plan, const Data& unisgnedEncodedCborData);
    /// Create a pseudo-random shuffle of numbers 0--(n-1).  Not true random, but reproducible.
    static std::vector<int> getShuffleMap(int n, int seed);
};

} // namespace TW::Cardano

/// Wrapper for C interface.
struct TWCardanoSigner {
    TW::Cardano::Signer impl;
};
