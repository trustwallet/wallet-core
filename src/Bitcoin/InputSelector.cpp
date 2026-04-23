// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "InputSelector.h"

#include "UTXO.h"
#include "Numeric.h"

#include <algorithm>
#include <optional>
#include <cassert>

namespace TW::Bitcoin {

template <typename TypeWithAmount>
Amount InputSelector<TypeWithAmount>::sum(const std::vector<TypeWithAmount>& amounts) {
    Amount sum = 0;
    for (auto& i : amounts) {
        sum = addUnsignedChecked(sum, i.amount);
    }
    return sum;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterOutDust(const std::vector<TypeWithAmount>& inputs,
                                             Amount byteFee) {
    auto dustThreshold = dustCalculator->dustAmount(byteFee);
    return filterThreshold(inputs, dustThreshold);
}

// Filters utxos that are dust
template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterThreshold(const std::vector<TypeWithAmount>& inputs,
                                               Amount minimumAmount) {
    std::vector<TypeWithAmount> filtered;
    for (auto& i : inputs) {
        if (static_cast<Amount>(i.amount) >= minimumAmount) {
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
InputSelector<TypeWithAmount>::select(Amount targetValue, Amount byteFee, size_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    std::vector<TypeWithAmount> sorted = filterOutDust(_inputs, byteFee);
    std::sort(
        sorted.begin(),
        sorted.end(),
        [](const TypeWithAmount& lhs, const TypeWithAmount& rhs) {
            return lhs.amount < rhs.amount;
        });

    // total values of utxos should be greater than targetValue
    if (sorted.empty() || sum(sorted) < targetValue) {
        return {};
    }
    assert(sorted.size() >= 1);

    // definitions for the following calculation
    const auto doubleTargetValue = mulUnsignedChecked(targetValue, 2ull);

    // Precompute maximum amount possible to obtain with given number of inputs
    const auto n = sorted.size();
    std::vector<Amount> maxWithXInputs = std::vector<Amount>();
    maxWithXInputs.push_back(0);
    Amount maxSum = 0;
    for (auto i = 0ul; i < n; ++i) {
        maxSum = addUnsignedChecked(maxSum, sorted[n - 1 - i].amount);
        maxWithXInputs.push_back(maxSum);
    }

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](Amount val) -> Amount {
        if (val > doubleTargetValue) {
            return val - doubleTargetValue;
        }
        return doubleTargetValue - val;
    };

    const Amount dustThreshold = dustCalculator->dustAmount(byteFee);

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
        auto slices = slice(sorted, numInputs);

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
            return slices.front();
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
            return slices.front();
        }
    }

    // If we couldn't find a combination of inputs to cover estimated transaction fee and the target amount,
    // return the whole set of UTXOs. Later, the transaction fee will be calculated more accurately,
    // and these UTXOs can be enough.
    return sorted;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount> InputSelector<TypeWithAmount>::selectSimple(Amount targetValue,
                                                                        Amount byteFee,
                                                                        size_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }
    if (_inputs.empty()) {
        return {};
    }
    assert(_inputs.size() >= 1);

    // target value is larger than original, but not by a factor of 2 (optimized for large UTXO
    // cases)
    const auto increasedTargetValue =
        (Amount)((double)targetValue * 1.1 +
                   feeCalculator.calculate(_inputs.size(), numOutputs, byteFee) + 1000);

    const Amount dustThreshold = dustCalculator->dustAmount(byteFee);

    // Go through inputs in a single pass, in the order they appear, no optimization
    Amount sum = 0;
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

    // If we couldn't find a combination of inputs to cover estimated transaction fee and the target amount,
    // return the whole set of UTXOs. Later, the transaction fee will be calculated more accurately,
    // and these UTXOs can be enough.
    return selected;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::selectMaxAmount(Amount byteFee) {
    return filterOutDust(_inputs, byteFee);
}

// Explicitly instantiate
template class Bitcoin::InputSelector<UTXO>;

} // namespace TW::Bitcoin
