// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "CoinEntry.h"
#include "Data.h"
#include "../proto/Nervos.pb.h"

namespace TW::Nervos {

class Signer {
public:
    Signer() = delete;

    /// Returns a transaction plan (utxo selection, fee estimation)
    static Proto::TransactionPlan plan(const Proto::SigningInput& signingInputProto) noexcept;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& signingInputProto) noexcept;
};

} // namespace TW::Nervos
