// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "UnspentSelector.h"

#include <algorithm>
#include <cassert>

using namespace TW;
using namespace TW::Bitcoin;

/// A selection of unspent transactions.
struct Selection {
    UTXOs utxos;
    int64_t total;
};

// Filters utxos that are dust
UTXOs UnspentSelector::filterDustInput(const UTXOs& selectedUtxos, int64_t byteFee) {
    auto inputFeeLimit = feeCalculator.calculateSingleInput(byteFee);
    UTXOs filteredUtxos;
    for (auto utxo: selectedUtxos) {
        if (utxo.amount > inputFeeLimit) {
            filteredUtxos.push_back(utxo);
        }
    }
    return filteredUtxos;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8],
// [7, 8, 9]]
static inline auto slice(const UTXOs& elements, size_t sliceSize) {
    std::vector<UTXOs> slices;
    for (auto i = 0; i <= elements.size() - sliceSize; i += 1) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j += 1) {
            slices[i].push_back(elements[j]);
        }
    }
    return slices;
}

UTXOs UnspentSelector::select(const UTXOs& utxos, int64_t targetValue, int64_t byteFee, int64_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }

    // total values of utxos should be greater than targetValue
    if (utxos.empty() || sum(utxos) < targetValue) {
        return {};
    }
    assert(utxos.size() >= 1);

    // definitions for the following caluculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    auto sortedUtxos = utxos;
    std::sort(sortedUtxos.begin(), sortedUtxos.end(),
              [](const UTXO& lhs, const UTXO& rhs) {
                  return lhs.amount < rhs.amount;
              });
    // Precompute maximum amount possible to obtain with given number of UTXOs
    const auto n = sortedUtxos.size();
    std::vector<uint64_t> maxWithXInputs = std::vector<uint64_t>();
    maxWithXInputs.push_back(0);
    int64_t maxSum = 0;
    for (auto i = 0; i < n; ++i) {
        maxSum += sortedUtxos[n - 1 - i].amount;
        maxWithXInputs.push_back(maxSum);
    }

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](int64_t val) -> int64_t {
        if (val > doubleTargetValue)
            return val - doubleTargetValue;
        else
            return doubleTargetValue - val;
    };

    const int64_t dustThreshold = feeCalculator.calculateSingleInput(byteFee);

    // 1. Find a combination of the fewest inputs that is
    //    (1) bigger than what we need
    //    (2) closer to 2x the amount,
    //    (3) and does not produce dust change.
    for (int64_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = feeCalculator.calculate(numInputs, numOutputs, byteFee);
        const auto targetWithFeeAndDust = targetValue + fee + dustThreshold;
        if (maxWithXInputs[numInputs] < targetWithFeeAndDust) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sortedUtxos, static_cast<size_t>(numInputs));
        slices.erase(
            std::remove_if(slices.begin(), slices.end(),
                [targetWithFeeAndDust](const UTXOs& slice) {
                    return sum(slice) < targetWithFeeAndDust;
                }),
            slices.end());
        if (!slices.empty()) {
            std::sort(slices.begin(), slices.end(),
                [distFrom2x](const UTXOs& lhs,
                            const UTXOs& rhs) {
                    return distFrom2x(sum(lhs)) < distFrom2x(sum(rhs));
                });
            return filterDustInput(slices.front(), byteFee);
        }
    }

    // 2. If not, find a valid combination of outputs even if they produce dust change.
    for (int64_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = feeCalculator.calculate(numInputs, numOutputs, byteFee);
        const auto targetWithFee = targetValue + fee;
        if (maxWithXInputs[numInputs] < targetWithFee) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sortedUtxos, static_cast<size_t>(numInputs));
        slices.erase(
            std::remove_if(slices.begin(), slices.end(),
                [targetWithFee](const UTXOs& slice) {
                    return sum(slice) < targetWithFee;
                }),
            slices.end());
        if (!slices.empty()) {
            return filterDustInput(slices.front(), byteFee);
        }
    }

    return {};
}

UTXOs UnspentSelector::selectMaxAmount(const UTXOs& utxos, int64_t byteFee) {
    return filterDustInput(utxos, byteFee);
}
