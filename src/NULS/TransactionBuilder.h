// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/NULS.pb.h"

#include <algorithm>

namespace TW::NULS {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Proto::TransactionPlan plan(const NULS::Proto::TransactionPurpose& purpose) {
        auto plan = Proto::TransactionPlan();
        plan.set_amount(purpose.amount());

        if (purpose.amount() == 0 || purpose.utxos_size() == 0) {
            return Proto::TransactionPlan{};
        }

        plan.set_timestamp(purpose.timestamp());
        plan.set_private_key(purpose.private_key());
        plan.set_from_address(purpose.from_address());
        plan.set_to_address(purpose.to_address());
        plan.set_remark(purpose.remark());

        int64_t availableAmount = 0;
        for (auto& utxo : purpose.utxos()) {
            availableAmount += utxo.amount();
        }
        plan.set_available_amount(availableAmount);

        if (availableAmount < plan.amount()) {
            return Proto::TransactionPlan{};
        }

        Proto::TransactionOutput output;
        output.set_amount(purpose.amount());
        output.set_to_address(purpose.to_address());
        output.set_index(0);
        output.set_lock_time(0);

        std::vector<Proto::TransactionOutput> outputs;
        outputs.push_back(output);

        auto sortedUtxos = purpose.utxos();
        std::sort(sortedUtxos.begin(), sortedUtxos.end(),
                  [](const Proto::TransactionInput& lhs, const Proto::TransactionInput& rhs) {
                      return lhs.amount() < rhs.amount();
                  });

        std::vector<Proto::TransactionInput> selectUtxos;
        int64_t values = 0;
        for (int i = 0; i < sortedUtxos.size(); ++i) {
            auto utxo = sortedUtxos[i];
            selectUtxos.push_back(utxo);
            values += utxo.amount();
            int64_t fee = calculatorTransactionFee(selectUtxos.size(), 1, purpose.remark().size());

            if (values > (purpose.amount() + fee)) {
                fee = calculatorTransactionFee(selectUtxos.size(), 2, purpose.remark().size());
                if (values < (purpose.amount() + fee)) {
                    continue;
                }

                Proto::TransactionOutput changeOutput;
                changeOutput.set_amount(values - purpose.amount() - fee);
                changeOutput.set_to_address(purpose.from_address());
                changeOutput.set_index(1);
                changeOutput.set_lock_time(0);
                outputs.push_back(changeOutput);
            }

            plan.set_fee(fee);
            plan.set_change(values - purpose.amount() - fee);

            if (values >= (purpose.amount() + fee)) {
                break;
            }
        }

        *plan.mutable_inputs() = {selectUtxos.begin(), selectUtxos.end()};
        *plan.mutable_outputs() = {outputs.begin(), outputs.end()};

        return plan;
    }

    static int64_t calculatorTransactionFee(uint32_t inputCount, uint32_t outputCount,
                                            uint32_t remarkSize) {
        uint64_t size = 124 + 50 * inputCount + 38 * outputCount + remarkSize;
        return calculateFee(size);
    }

    static const uint64_t MIN_PRICE_PRE_1024_BYTES = 100000;

    static inline uint64_t calculateFee(uint64_t size) {
        uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
        if (size % 1024 > 0) {
            fee += MIN_PRICE_PRE_1024_BYTES;
        }
        return fee;
    }
};

} // namespace TW::NULS
