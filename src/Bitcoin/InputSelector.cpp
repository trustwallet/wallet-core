// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InputSelector.h"

#include "UTXO.h"

#include <algorithm>
#include <optional>
#include <cassert>

namespace TW::Bitcoin {

template <typename TypeWithAmount>
uint64_t InputSelector<TypeWithAmount>::sum(const std::vector<TypeWithAmount>& amounts) noexcept {
    uint64_t sum = 0;
    for (auto& i : amounts) {
        sum += i.amount;
    }
    return sum;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterOutDust(const std::vector<TypeWithAmount>& inputs,
                                             int64_t byteFee) noexcept {
    auto inputFeeLimit = static_cast<uint64_t>(feeCalculator.calculateSingleInput(byteFee));
    return filterThreshold(inputs, inputFeeLimit);
}

// Filters utxos that are dust
template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterThreshold(const std::vector<TypeWithAmount>& inputs,
                                               uint64_t minimumAmount) noexcept {
    std::vector<TypeWithAmount> filtered;
    for (auto& i : inputs) {
        if (static_cast<uint64_t>(i.amount) > minimumAmount) {
            filtered.push_back(i);
        }
    }
    return filtered;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8],
// [7, 8, 9]]
template <typename TypeWithAmount>
static inline std::vector<std::vector<TypeWithAmount>>
slice(const std::vector<TypeWithAmount>& inputs, size_t sliceSize) {
    std::vector<std::vector<TypeWithAmount>> slices;
    for (auto i = 0ul; i <= inputs.size() - sliceSize; ++i) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j++) {
            slices[i].push_back(inputs[j]);
        }
    }
    return slices;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::select(uint64_t targetValue, uint64_t byteFee, uint64_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }

    // total values of utxos should be greater than targetValue
    if (_inputs.empty() || sum(_inputs) < targetValue) {
        return {};
    }
    assert(_inputs.size() >= 1);

    // definitions for the following calculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    std::vector<TypeWithAmount> sorted = _inputs;
    std::sort(sorted.begin(), sorted.end(),
              [](const TypeWithAmount& lhs, const TypeWithAmount& rhs) {
                  return lhs.amount < rhs.amount;
              });

    // Precompute maximum amount possible to obtain with given number of inputs
    const auto n = sorted.size();
    std::vector<uint64_t> maxWithXInputs = std::vector<uint64_t>();
    maxWithXInputs.push_back(0);
    int64_t maxSum = 0;
    for (auto i = 0ul; i < n; ++i) {
        maxSum += sorted[n - 1 - i].amount;
        maxWithXInputs.push_back(maxSum);
    }

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](uint64_t val) -> uint64_t {
        if (val > doubleTargetValue) {
            return val - doubleTargetValue;
        }
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
        auto slices = slice(sorted, static_cast<size_t>(numInputs));

        slices.erase(
            std::remove_if(slices.begin(), slices.end(),
                           [targetWithFeeAndDust](const std::vector<TypeWithAmount>& slice) {
                               return sum(slice) < targetWithFeeAndDust;
                           }),
            slices.end());
        if (!slices.empty()) {
            std::sort(slices.begin(), slices.end(),
                      [distFrom2x](const std::vector<TypeWithAmount>& lhs,
                                   const std::vector<TypeWithAmount>& rhs) {
                          return distFrom2x(sum(lhs)) < distFrom2x(sum(rhs));
                      });
            return filterOutDust(slices.front(), byteFee);
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
        auto slices = slice(sorted, static_cast<size_t>(numInputs));
        slices.erase(std::remove_if(slices.begin(), slices.end(),
                                    [targetWithFee](const std::vector<TypeWithAmount>& slice) {
                                        return sum(slice) < targetWithFee;
                                    }),
                     slices.end());
        if (!slices.empty()) {
            return filterOutDust(slices.front(), byteFee);
        }
    }

    return {};
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount> InputSelector<TypeWithAmount>::selectSimple(int64_t targetValue,
                                                                        int64_t byteFee,
                                                                        int64_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }
    if (_inputs.empty()) {
        return {};
    }
    assert(_inputs.size() >= 1);

    // target value is larger that original, but not by a factor of 2 (optimized for large UTXO
    // cases)
    const auto increasedTargetValue =
        (uint64_t)((double)targetValue * 1.1 +
                   feeCalculator.calculate(_inputs.size(), numOutputs, byteFee) + 1000);

    const int64_t dustThreshold = feeCalculator.calculateSingleInput(byteFee);

    // Go through inputs in a single pass, in the order they appear, no optimization
    uint64_t sum = 0;
    std::vector<TypeWithAmount> selected;
    for (auto& input : _inputs) {
        if (input.amount <= dustThreshold) {
            continue; // skip dust
        }
        selected.push_back(input);
        sum += input.amount;
        if (sum >= increasedTargetValue) {
            // we have enough
            return selected;
        }
    }

    // not enough
    return {};
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::selectMaxAmount(int64_t byteFee) noexcept {
    return filterOutDust(_inputs, byteFee);
}

// Explicitly instantiate
template class Bitcoin::InputSelector<UTXO>;

} // namespace TW::Bitcoin
