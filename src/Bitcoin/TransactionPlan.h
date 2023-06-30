// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Amount.h"
#include "UTXO.h"
#include "Data.h"
#include "../proto/Bitcoin.pb.h"

namespace TW::Bitcoin {

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

    /// Selected unspent transaction outputs.
    UTXOs utxos;

    /// Zcash branch id
    Data branchId;

    /// zen & bitcoin diamond preblockhash
    Data preBlockHash;

    /// zen preblockheight
    int64_t preBlockHeight = 0;

    Data outputOpReturn;

    Common::Proto::SigningError error = Common::Proto::SigningError::OK;

    TransactionPlan() = default;

    TransactionPlan(const Proto::TransactionPlan& plan)
        : amount(plan.amount())
        , availableAmount(plan.available_amount())
        , fee(plan.fee())
        , change(plan.change())
        , utxos(std::vector<UTXO>(plan.utxos().begin(), plan.utxos().end()))
        , branchId(plan.branch_id().begin(), plan.branch_id().end())
        , preBlockHash(plan.preblockhash().begin(), plan.preblockhash().end())
        , preBlockHeight(plan.preblockheight())
        , outputOpReturn(plan.output_op_return().begin(), plan.output_op_return().end())
        , error(plan.error())
    {}

    Proto::TransactionPlan proto() const {
        auto plan = Proto::TransactionPlan();
        plan.set_amount(amount);
        plan.set_available_amount(availableAmount);
        plan.set_fee(fee);
        plan.set_change(change);
        for (auto& utxo: utxos) {
            *plan.add_utxos() = utxo.proto();
        }
        plan.set_branch_id(branchId.data(), branchId.size());
        plan.set_preblockhash(preBlockHash.data(), preBlockHash.size());
        plan.set_preblockheight(preBlockHeight);
        plan.set_output_op_return(outputOpReturn.data(), outputOpReturn.size());
        plan.set_error(error);
        return plan;
    }
};

} // namespace TW::Bitcoin
