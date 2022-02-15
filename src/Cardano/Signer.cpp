// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include <vector>

using namespace TW::Cardano;
using namespace TW;

Proto::SigningOutput Signer::sign() {
    // plan if needed
    // currently there is no way to specify plan from outside, TODO
    _plan = plan();

    auto ret = Proto::SigningOutput();
    // TODO
    return ret;
}

TransactionPlan Signer::plan() const {
    auto utxos = std::vector<Utxo>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        utxos.push_back(Utxo::fromProto(input.utxos(i)));
    }

    auto plan = TransactionPlan();

    for (auto& u: utxos) {
        plan.availableAmount += u.amount;
    }
    plan.amount = input.transfer_message().amount(); // TODO case?
    plan.amount = std::min(plan.availableAmount, plan.amount);
    plan.fee = 0; // TODO
    plan.fee = std::min(plan.availableAmount - plan.amount, plan.fee);
    plan.change = plan.availableAmount - plan.amount - plan.fee;

    return plan;
}
