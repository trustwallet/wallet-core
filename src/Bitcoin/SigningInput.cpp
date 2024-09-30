// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "SigningInput.h"

namespace TW::Bitcoin {

SigningInput::SigningInput()
    : dustCalculator(std::make_shared<LegacyDustCalculator>(TWCoinTypeBitcoin)) {
}

SigningInput::SigningInput(const Proto::SigningInput& input) {
    hashType = static_cast<TWBitcoinSigHashType>(input.hash_type());
    amount = input.amount();
    byteFee = input.byte_fee();
    toAddress = input.to_address();
    changeAddress = input.change_address();
    for (auto&& key : input.private_key()) {
        privateKeys.emplace_back(key);
    }
    for (auto&& script : input.scripts()) {
        scripts[script.first] = Script(script.second.begin(), script.second.end());
    }
    for (auto&& u : input.utxo()) {
        utxos.emplace_back(u);
    }
    useMaxAmount = input.use_max_amount();
    useMaxUtxo = input.use_max_utxo();
    disableDustFilter = input.disable_dust_filter();
    coinType = static_cast<TWCoinType>(input.coin_type());
    if (input.has_plan()) {
        plan = TransactionPlan(input.plan());
    }
    outputOpReturn = data(input.output_op_return());
    if (input.has_output_op_return_index()) {
        outputOpReturnIndex = input.output_op_return_index().index();
    }
    lockTime = input.lock_time();
    time = input.time();

    extraOutputsAmount = 0;
    for (auto& output: input.extra_outputs()) {
        extraOutputsAmount += output.amount();
        extraOutputs.push_back(std::make_pair(output.to_address(), output.amount()));
    }

    dustCalculator = getDustCalculator(input);
}

} // namespace TW::Bitcoin
