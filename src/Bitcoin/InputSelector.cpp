// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InputSelector.h"

#include <algorithm>
#include <cassert>

using namespace TW;
using namespace TW::Bitcoin;


uint64_t InputSelector::sum(const std::vector<uint64_t>& amounts) {
    return std::accumulate(amounts.begin(), amounts.end(), 0);
}

inline int64_t InputSelector::sumIndices(const std::vector<size_t>& indices) {
    int64_t sum = 0;
    for (auto i : indices) {
        sum += inputs[i];
    }
    return sum;
}

// Filters utxos that are dust
std::vector<size_t> InputSelector::filterDustInput(const std::vector<size_t>& inputIndices, int64_t byteFee) {
    auto inputFeeLimit = feeCalculator.calculateSingleInput(byteFee);
    std::vector<size_t> filteredIndices;
    for (auto i: inputIndices) {
        if (inputs[i] > inputFeeLimit) {
            filteredIndices.push_back(i);
        }
    }
    return filteredIndices;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8],
// [7, 8, 9]]
static inline std::vector<std::vector<size_t>> slice(const std::vector<size_t>& indices, size_t sliceSize) {
    std::vector<std::vector<size_t>> slices;
    for (auto i = 0; i <= indices.size() - sliceSize; ++i) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j++) {
            slices[i].push_back(indices[j]);
        }
    }
    return slices;
}

std::vector<size_t> InputSelector::select(int64_t targetValue, int64_t byteFee, int64_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }

    // total values of utxos should be greater than targetValue
    if (inputs.empty() || sum(inputs) < targetValue) {
        return {};
    }
    assert(inputs.size() >= 1);

    // definitions for the following caluculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    std::vector<size_t> sortedIndices;
    for (auto i = 0; i < inputs.size(); ++i) {
        sortedIndices.push_back(i);
    }
    std::sort(sortedIndices.begin(), sortedIndices.end(),
        [this](const size_t& lhs, const size_t& rhs) {
            return inputs[lhs] < inputs[rhs];
        });

    // Precompute maximum amount possible to obtain with given number of inputs
    const auto n = sortedIndices.size();
    std::vector<uint64_t> maxWithXInputs = std::vector<uint64_t>();
    maxWithXInputs.push_back(0);
    int64_t maxSum = 0;
    for (auto i = 0; i < n; ++i) {
        maxSum += inputs[sortedIndices[n - 1 - i]];
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
    for (size_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = feeCalculator.calculate(numInputs, numOutputs, byteFee);
        const auto targetWithFeeAndDust = targetValue + fee + dustThreshold;
        if (maxWithXInputs[numInputs] < targetWithFeeAndDust) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sortedIndices, static_cast<size_t>(numInputs));

        slices.erase(
            std::remove_if(slices.begin(), slices.end(),
                [this, targetWithFeeAndDust](const std::vector<size_t>& slice) {
                    return sumIndices(slice) < targetWithFeeAndDust;
                }),
            slices.end());
        if (!slices.empty()) {
            std::sort(slices.begin(), slices.end(),
                [this, distFrom2x](const std::vector<size_t>& lhs, const std::vector<size_t>& rhs) {
                    return distFrom2x(sumIndices(lhs)) < distFrom2x(sumIndices(rhs));
                });
            return filterDustInput(slices.front(), byteFee);
        }
    }

    // 2. If not, find a valid combination of outputs even if they produce dust change.
    for (size_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = feeCalculator.calculate(numInputs, numOutputs, byteFee);
        const auto targetWithFee = targetValue + fee;
        if (maxWithXInputs[numInputs] < targetWithFee) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sortedIndices, static_cast<size_t>(numInputs));
        slices.erase(
            std::remove_if(slices.begin(), slices.end(),
                [this,  targetWithFee](const std::vector<size_t>& slice) {
                    return sumIndices(slice) < targetWithFee;
                }),
            slices.end());
        if (!slices.empty()) {
            return filterDustInput(slices.front(), byteFee);
        }
    }

    return {};
}

std::vector<size_t> InputSelector::selectMaxAmount(int64_t byteFee) {
    std::vector<size_t> indices;
    for (auto i = 0; i < inputs.size(); ++i) {
        indices.push_back(i);
    }
    return filterDustInput(indices, byteFee);
}
