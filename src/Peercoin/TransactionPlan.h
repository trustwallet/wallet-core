// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Amount.h"
#include "../proto/Peercoin.pb.h"

namespace TW::Peercoin {

/// Describes a preliminary transaction plan.
struct TransactionPlan {
    /// Amount to be received at the other end.
    Amount amount = 0;

    /// Maximum available amount.
    Amount availableAmount = 0;

    /// Estimated transaction fee.
    Amount fee = 0;

    /// Change.
    Amount change = 0;

    /// timestamp of resulting transaction
    int timestamp = 0;

    /// Selected unspent transaction outputs.
    std::vector<Peercoin::Proto::UnspentTransaction> utxos;

    TransactionPlan() = default;

    TransactionPlan(const Proto::TransactionPlan& plan)
        : amount(plan.amount())
        , availableAmount(plan.available_amount())
        , fee(plan.fee())
        , change(plan.change())
        , timestamp(plan.timestamp())
        , utxos(plan.utxos().begin(), plan.utxos().end()) {}

    Proto::TransactionPlan proto() const {
        auto plan = Proto::TransactionPlan();
        plan.set_amount(amount);
        plan.set_available_amount(availableAmount);
        plan.set_fee(fee);
        plan.set_change(change);
        plan.set_timestamp(timestamp);
        *plan.mutable_utxos() = {utxos.begin(), utxos.end()};
        return plan;
    }
};

} // namespace TW::Peercoin
