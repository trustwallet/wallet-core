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
        if (purpose.utxos_size() == 0) {
            return Proto::TransactionPlan{};
        }

        if (purpose.use_max_amount()) {
            return calculatorMaxAmountPlan(purpose);
        }

        if (purpose.amount() == 0) {
            return Proto::TransactionPlan{};
        }

        // set common field first
        auto plan = Proto::TransactionPlan();
        plan.set_amount(purpose.amount());
        plan.set_timestamp(purpose.timestamp());
        plan.set_private_key(purpose.private_key());
        plan.set_from_address(purpose.from_address());
        plan.set_to_address(purpose.to_address());
        plan.set_remark(purpose.remark());

        // Calculate all UTXO available amount
        uint64_t availableAmount = 0;
        for (auto& utxo : purpose.utxos()) {
            availableAmount += utxo.amount();
        }
        plan.set_available_amount(availableAmount);

        // all UTXO available amount less that transfer amount, error
        if (!purpose.use_max_amount() && availableAmount < plan.amount()) {
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
                      return lhs.amount() > rhs.amount();
                  });

        std::vector<Proto::TransactionInput> selectUtxos;
        int64_t values = 0;
        int64_t fee = 0;
        for (auto utxo : sortedUtxos) {
            selectUtxos.push_back(utxo);
            values += utxo.amount();
            // We need recalculate transaction fee when we insert an new utxo record
            uint32_t txSize =
                calculatorTransactionSize(static_cast<uint32_t>(selectUtxos.size()), 1, static_cast<uint32_t>(purpose.remark().size()));
            fee = calculatorTransactionFee(txSize);

            if (values == (purpose.amount() + fee)) {
                // selected uxto amount equal spent and fee.
                break;
            } else if (values > (purpose.amount() + fee)) {
                // all amount bigger that spent and fee, need change back

                txSize = calculatorTransactionSize(static_cast<uint32_t>(selectUtxos.size()), 2, static_cast<uint32_t>(purpose.remark().size()));
                fee = calculatorTransactionFee(txSize);
                if (values < (purpose.amount() + fee)) {
                    // don't have enough input amount because change back output
                    continue;
                }

                Proto::TransactionOutput changeOutput;
                changeOutput.set_amount(values - purpose.amount() - fee);
                changeOutput.set_to_address(purpose.from_address());
                changeOutput.set_index(1);
                changeOutput.set_lock_time(0);
                outputs.push_back(changeOutput);
                break;
            } else {
                if (txSize > MAX_TRANSACTION_SIZE) {
                    // Can't execute purpose transaction because transaction size limit
                    return Proto::TransactionPlan{};
                }
            }
        }

        plan.set_fee(fee);
        plan.set_change(values - purpose.amount() - fee);

        *plan.mutable_inputs() = {selectUtxos.begin(), selectUtxos.end()};
        *plan.mutable_outputs() = {outputs.begin(), outputs.end()};

        return plan;
    }

    static Proto::TransactionPlan
    calculatorMaxAmountPlan(const NULS::Proto::TransactionPurpose& purpose) {
        auto plan = Proto::TransactionPlan();

        uint32_t maxInputs = calculatorMaxInput(static_cast<uint32_t>(purpose.remark().size()));
        uint32_t selectCount = std::min((uint32_t)purpose.utxos_size(), maxInputs);

        auto sortedUtxos = purpose.utxos();
        std::sort(sortedUtxos.begin(), sortedUtxos.end(),
            [](const Proto::TransactionInput& lhs, const Proto::TransactionInput& rhs) {
              return lhs.amount() > rhs.amount();
            });

        std::vector<Proto::TransactionInput> selectUtxos;
        uint64_t newAmount = 0;
        for (int i = 0; i < selectCount; ++i) {
            auto utxo = sortedUtxos[i];
            newAmount += utxo.amount();
            selectUtxos.push_back(utxo);
        }

        uint32_t txSize = calculatorTransactionSize(selectCount, 1, static_cast<uint32_t>(purpose.remark().size()));
        uint64_t fee = calculatorTransactionFee(txSize);
        if (newAmount < fee) {
            return Proto::TransactionPlan{};
        }

        plan.set_available_amount(newAmount);
        plan.set_amount(newAmount - fee);
        plan.set_fee(fee);
        plan.set_change(0L);

        Proto::TransactionOutput output;
        output.set_amount(plan.amount());
        output.set_to_address(purpose.to_address());
        output.set_index(0);
        output.set_lock_time(0);

        std::vector<Proto::TransactionOutput> outputs;
        outputs.push_back(output);

        *plan.mutable_inputs() = {selectUtxos.begin(), selectUtxos.end()};
        *plan.mutable_outputs() = {outputs.begin(), outputs.end()};
        return plan;
    }

    static int32_t calculatorMaxInput(uint32_t remarkSize) {
        uint32_t outputSize = 1 * TRANSACTION_OUTPUT_SIZE;
        uint32_t maxInputs =
            (MAX_TRANSACTION_SIZE - TRANSACTION_HEAD_SIZE - remarkSize - outputSize) / 50;
        if ((MAX_TRANSACTION_SIZE - TRANSACTION_HEAD_SIZE - remarkSize - outputSize) % 50 != 0) {
            maxInputs -= 1;
        }
        return maxInputs;
    }

    static uint32_t calculatorTransactionSize(uint32_t inputCount, uint32_t outputCount,
                                             uint32_t remarkSize) {
        uint32_t size = 124 + TRANSACTION_INPUT_SIZE * inputCount +
                        TRANSACTION_OUTPUT_SIZE * outputCount + remarkSize;
        return size;
    }

    static const uint16_t TRANSACTION_HEAD_SIZE = 124;
    static const uint16_t TRANSACTION_INPUT_SIZE = 50;
    static const uint16_t TRANSACTION_OUTPUT_SIZE = 38;
    /// Transaction size must less that 300KB
    static const uint64_t MAX_TRANSACTION_SIZE = 300 * 1024;
    /// 0.001 NULS per KB
    static const uint64_t MIN_PRICE_PRE_1024_BYTES = 100000;

    static inline uint64_t calculatorTransactionFee(uint64_t size) {
        uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
        if (size % 1024 > 0) {
            fee += MIN_PRICE_PRE_1024_BYTES;
        }
        return fee;
    }
};

} // namespace TW::NULS
