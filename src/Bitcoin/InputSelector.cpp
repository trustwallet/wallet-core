// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InputSelector.h"

#include "NumericLiteral.h"
#include "UTXO.h"
#include "algorithm/erase.h"
#include "algorithm/sort_copy.h"

#include <algorithm>
#include <cassert>
#include <optional>

using namespace TW;
using namespace TW::Bitcoin;

using MaybeDust = std::optional<int64_t>;
using MaybeFee = std::optional<int64_t>;
using Inputs = std::vector<uint64_t>;

namespace details {

struct FeeCalculatorArgs {
    int64_t inputs;
    int64_t outputs;
    int64_t byteFee;
};

static inline int64_t distFrom2x(int64_t doubleTargetValue, int64_t val) noexcept {
    return val > doubleTargetValue ? val - doubleTargetValue : doubleTargetValue - val;
}

template <typename TypeWithAmount>
static inline std::vector<TypeWithAmount>
selectOutDustElement(int64_t doubleTargetValue, MaybeDust dustThreshold,
                     const std::vector<std::vector<TypeWithAmount>>& slices) noexcept {
    if (dustThreshold.has_value()) {
        const auto minFunctor = [doubleTargetValue](auto&& lhs, auto&& rhs) noexcept {
            return details::distFrom2x(doubleTargetValue, InputSelector<TypeWithAmount>::sum(lhs)) <
                   details::distFrom2x(doubleTargetValue, InputSelector<TypeWithAmount>::sum(rhs));
        };
        const auto min = std::min_element(cbegin(slices), cend(slices), minFunctor);
        return *min;
    }
    return slices.front();
}

static MaybeFee calculateTargetFee(const Inputs& maxWithXInputs, const FeeCalculator& feeCalculator,
                                   FeeCalculatorArgs feeCalculatorArgs, int64_t targetValue,
                                   MaybeDust dust = std::nullopt) noexcept {
    auto&& [numInputs, numOutputs, byteFee] = feeCalculatorArgs;
    const auto fee = feeCalculator.calculate(numInputs, numOutputs, byteFee);
    auto targetFee = targetValue + fee;
    if (dust) {
        targetFee += *dust;
    }
    if (maxWithXInputs[numInputs] < targetFee) {
        // no way to satisfy with only numInputs inputs, skip
        return std::nullopt;
    }
    return std::make_optional(targetFee);
}

// Precompute maximum amount possible to obtain with given number of inputs
template <typename TypeWithAmount>
static inline Inputs collectMaxWithXInputs(const std::vector<TypeWithAmount>& sorted) noexcept {
    Inputs maxWithXInputs(sorted.size() + 1);
    std::transform_inclusive_scan(crbegin(sorted), crend(sorted), next(begin(maxWithXInputs)),
                                  std::plus<>{}, std::mem_fn(&TypeWithAmount::amount), uint64_t{});
    return maxWithXInputs;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8],
// [7, 8, 9]]
template <typename TypeWithAmount>
static inline std::vector<std::vector<TypeWithAmount>>
sliding(const std::vector<TypeWithAmount>& inputs, size_t sliceSize) noexcept {
    std::vector<std::vector<TypeWithAmount>> slices;
    auto&& [from, to] = std::make_pair(cbegin(inputs), cend(inputs) - sliceSize);
    slices.reserve(std::distance(from, to));
    for (auto&& it = std::move(from); it <= to; ++it) {
        slices.emplace_back(it, it + sliceSize);
    }
    return slices;
}

} // namespace details

template <typename TypeWithAmount>
uint64_t InputSelector<TypeWithAmount>::sum(const std::vector<TypeWithAmount>& amounts) noexcept {
    auto accumulateFunctor = [](std::size_t sum, auto&& cur) noexcept { return sum + cur.amount; };
    return std::accumulate(cbegin(amounts), cend(amounts), 0_uz, accumulateFunctor);
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
    auto copyFunctor = [minimumAmount](auto&& cur) noexcept { return cur.amount > minimumAmount; };
    std::copy_if(cbegin(inputsIn), cend(inputsIn), std::back_inserter(filtered), copyFunctor);
    return filtered;
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

    // Get all possible utxo selections up to a maximum size, sort by total amount, increasing
    const auto sortFunctor = [](auto&& l, auto&& r) noexcept { return l.amount < r.amount; };
    const auto sorted = sortCopy(inputs, sortFunctor);
    const auto n = sorted.size();
    const auto maxWithXInputs = details::collectMaxWithXInputs<TypeWithAmount>(sorted);

    auto sliceFunctor = [&sorted](int64_t numInputs, int64_t targetFee) noexcept {
        auto slices = details::sliding(sorted, static_cast<size_t>(numInputs));
        auto eraseFunctor = [targetFee](auto&& slice) noexcept { return sum(slice) < targetFee; };
        erase_if(slices, eraseFunctor);
        return slices;
    };

    auto selectFunctor =
        [numOutputs, byteFee, &maxWithXInputs, targetValue, &sliceFunctor,
         this](int64_t numInputs, MaybeDust dustThreshold = std::nullopt) noexcept
        -> std::optional<std::vector<TypeWithAmount>> {
            auto feeArgs = details::FeeCalculatorArgs{
                .inputs = numInputs, .outputs = numOutputs, .byteFee = byteFee};
            auto maybeTargetFee = details::calculateTargetFee(maxWithXInputs, feeCalculator,
                                                              feeArgs, targetValue, dustThreshold);
            if (maybeTargetFee) {
                auto slices = sliceFunctor(numInputs, *maybeTargetFee);
                if (!slices.empty()) {
                    const auto doubleTargetValue = targetValue * 2;
                    const auto filteredInputs = filterOutDust(
                        details::selectOutDustElement(doubleTargetValue, dustThreshold, slices),
                        byteFee);
                    return std::make_optional(filteredInputs);
                }
            }
            return std::nullopt;
        };

    const int64_t dustThreshold = feeCalculator.calculateSingleInput(byteFee);

    // 1. Find a combination of the fewest inputs that is
    //    (1) bigger than what we need
    //    (2) closer to 2x the amount,
    //    (3) and does not produce dust change.
    for (int64_t numInputs = 1; numInputs <= n; ++numInputs) {
        if (auto selected = selectFunctor(numInputs, dustThreshold); selected) {
            return *selected;
        }
    }

    // 2. If not, find a valid combination of outputs even if they produce dust change.
    for (int64_t numInputs = 1; numInputs <= n; ++numInputs) {
        if (auto selected = selectFunctor(numInputs); selected) {
            return *selected;
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
