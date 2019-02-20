// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <numeric>
#include <vector>

#include "../proto/Bitcoin.pb.h"

namespace TW {
namespace Bitcoin {

class UnspentSelector {
public:
    /// Maximum allowable transaction dust.
    static const int64_t dustThreshold;

    /// Selects unspent transactions to use given a target transaction value.
    ///
    /// \returns the list of selected utxos or an empty list if there are insufficient funds.
    template<typename T>
    static std::vector<Proto::UnspentTransaction> select(const T& utxos, int64_t targetValue, int64_t byteFee, int64_t numOutputs = 2);

    static int64_t calculateFee(size_t inputs, size_t outputs = 2, int64_t byteFee = 1);

    static int64_t calculateSingleInputFee(int64_t byteFee);

    template<typename T>
    static inline int64_t sum(const T& utxos) {
        int64_t sum = 0;
        for (auto& utxo : utxos)
            sum += utxo.amount();
        return sum;
    }
};

}} // namespace
