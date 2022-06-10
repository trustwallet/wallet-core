// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InputSelector.h"

#include "NumericLiteral.h"
#include "UTXO.h"

#include <algorithm>
#include <cassert>

using namespace TW;
using namespace TW::Bitcoin;

namespace details {

// Precompute maximum amount possible to obtain with given number of inputs
template <typename TypeWithAmount>
std::vector<uint64_t> collectMaxWithXInputs(const std::vector<TypeWithAmount>& sorted) {
    std::vector<uint64_t> maxWithXInputs{0};
    maxWithXInputs.reserve(sorted.size());
    std::transform_inclusive_scan(crbegin(sorted), crend(sorted),
                                  std::back_inserter(maxWithXInputs), std::plus<>{},
                                  std::mem_fn(&TypeWithAmount::amount), std::uint64_t{});
    return maxWithXInputs;
}

} // namespace details

template <typename TypeWithAmount>
uint64_t InputSelector<TypeWithAmount>::sum(const std::vector<TypeWithAmount>& amounts) noexcept {
    auto accumulateFunctor = [](std::size_t sum, auto&& cur) { return sum + cur.amount; };
    return std::accumulate(begin(amounts), end(amounts), 0_uz, accumulateFunctor);
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterOutDust(const std::vector<TypeWithAmount>& inputsIn,
                                             int64_t byteFee) noexcept {
    auto inputFeeLimit = static_cast<uint64_t>(feeCalculator.calculateSingleInput(byteFee));
    return filterThreshold(inputsIn, inputFeeLimit);
}

// Filters utxos that are dust
template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::filterThreshold(const std::vector<TypeWithAmount>& inputsIn,
                                               uint64_t minimumAmount) noexcept {
    std::vector<TypeWithAmount> filtered;
    auto copyFunctor = [minimumAmount](auto&& cur) { return cur.amount > minimumAmount; };
    std::copy_if(begin(inputsIn), end(inputsIn), std::back_inserter(filtered), copyFunctor);
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
    for (auto i = 0; i <= inputs.size() - sliceSize; ++i) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j++) {
            slices[i].emplace_back(inputs[j]);
        }
    }
    return slices;
}

template <typename TypeWithAmount>
std::vector<TypeWithAmount>
InputSelector<TypeWithAmount>::select(int64_t targetValue, int64_t byteFee, int64_t numOutputs) {
    // if target value is zero, no UTXOs are needed
    if (targetValue == 0) {
        return {};
    }

    // total values of utxos should be greater than targetValue
    if (inputs.empty() || sum(inputs) < targetValue) {
        return {};
    }
    assert(inputs.size() >= 1);

    // definitions for the following calculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    std::vector<TypeWithAmount> sorted = inputs;
    std::sort(sorted.begin(), sorted.end(),
              [](auto&& lhs, auto&& rhs) { return lhs.amount < rhs.amount; });

    const auto n = sorted.size();
    const auto maxWithXInputs = details::collectMaxWithXInputs<TypeWithAmount>(sorted);

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](int64_t val) -> int64_t {
        return val > doubleTargetValue ? val - doubleTargetValue : doubleTargetValue - val;
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
    if (inputs.empty()) {
        return {};
    }

    // target value is larger that original, but not by a factor of 2 (optimized for large UTXO
    // cases)
    const auto increasedTargetValue =
        (uint64_t)((double)targetValue * 1.1 +
                   feeCalculator.calculate(inputs.size(), numOutputs, byteFee) + 1000);

    const int64_t dustThreshold = feeCalculator.calculateSingleInput(byteFee);

    // Go through inputs in a single pass, in the order they appear, no optimization
    uint64_t sum = 0;
    std::vector<TypeWithAmount> selected;
    for (auto&& input : inputs) {
        if (input.amount <= dustThreshold) {
            continue; // skip dust
        }
        selected.emplace_back(input);
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
    return filterOutDust(inputs, byteFee);
}

// Explicitly instantiate
template class Bitcoin::InputSelector<UTXO>;
