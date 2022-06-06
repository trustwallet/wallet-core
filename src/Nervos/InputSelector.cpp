// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InputSelector.h"

#include <algorithm>

using namespace TW;
using namespace TW::Nervos;

int64_t InputSelector::sum(const Cells& cells) {
    int64_t sum = 0;
    for (auto& cell : cells) {
        sum += cell.capacity;
    }
    return sum;
}

Cells InputSelector::filterOutDust(const Cells& inputs, int64_t sizeOfSingleInputAndWitness,
                                   int64_t byteFee) {
    auto inputFeeLimit = sizeOfSingleInputAndWitness * byteFee;
    return filterThreshold(inputs, inputFeeLimit);
}

// Filters cells that are dust
Cells InputSelector::filterThreshold(const Cells& cells, int64_t minimumAmount) {
    Cells filtered;
    for (auto& cell : cells) {
        if (cell.capacity > minimumAmount) {
            filtered.push_back(cell);
        }
    }
    return filtered;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8],
// [7, 8, 9]]
static inline std::vector<Cells> slice(const Cells& cells, size_t sliceSize) {
    std::vector<Cells> slices;
    for (auto i = 0; i <= cells.size() - sliceSize; ++i) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j++) {
            slices[i].push_back(cells[j]);
        }
    }
    return slices;
}

Cells InputSelector::select(int64_t targetValue, int64_t sizeWithoutInputs,
                            int64_t sizeOfSingleInputAndWitness, int64_t byteFee) {
    // if target value is zero, no Cells are needed
    if (targetValue == 0) {
        return {};
    }

    // total values of cells should be greater than targetValue
    if (inputs.empty() || sum(inputs) < targetValue) {
        return {};
    }

    // definitions for the following caluculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible cell selections up to a maximum size, sort by total capacity, increasing
    Cells sorted = inputs;
    std::sort(sorted.begin(), sorted.end(),
              [](const Cell& lhs, const Cell& rhs) { return lhs.capacity < rhs.capacity; });

    // Precompute maximum capacity possible to obtain with given number of inputs
    const auto n = sorted.size();
    std::vector<int64_t> maxWithXInputs = std::vector<int64_t>();
    maxWithXInputs.push_back(0);
    int64_t maxSum = 0;
    for (auto i = 0; i < n; ++i) {
        maxSum += sorted[n - 1 - i].capacity;
        maxWithXInputs.push_back(maxSum);
    }

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](int64_t val) -> int64_t {
        if (val > doubleTargetValue) {
            return val - doubleTargetValue;
        }
        return doubleTargetValue - val;
    };

    const int64_t dustThreshold = sizeOfSingleInputAndWitness * byteFee;

    // 1. Find a combination of the fewest inputs that is
    //    (1) bigger than what we need
    //    (2) closer to 2x the amount,
    //    (3) and does not produce dust change.
    for (size_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = (sizeWithoutInputs + numInputs * sizeOfSingleInputAndWitness) * byteFee;
        const auto targetWithFeeAndDust = targetValue + fee + dustThreshold;
        if (maxWithXInputs[numInputs] < targetWithFeeAndDust) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sorted, static_cast<size_t>(numInputs));

        slices.erase(std::remove_if(slices.begin(), slices.end(),
                                    [targetWithFeeAndDust](const Cells& slice) {
                                        return sum(slice) < targetWithFeeAndDust;
                                    }),
                     slices.end());
        if (!slices.empty()) {
            std::sort(slices.begin(), slices.end(),
                      [distFrom2x](const Cells& lhs, const Cells& rhs) {
                          return distFrom2x(sum(lhs)) < distFrom2x(sum(rhs));
                      });
            return filterOutDust(slices.front(), sizeOfSingleInputAndWitness, byteFee);
        }
    }

    // 2. If not, find a valid combination of outputs even if they produce dust change.
    for (size_t numInputs = 1; numInputs <= n; ++numInputs) {
        const auto fee = (sizeWithoutInputs + numInputs * sizeOfSingleInputAndWitness) * byteFee;
        const auto targetWithFee = targetValue + fee;
        if (maxWithXInputs[numInputs] < targetWithFee) {
            // no way to satisfy with only numInputs inputs, skip
            continue;
        }
        auto slices = slice(sorted, static_cast<size_t>(numInputs));
        slices.erase(std::remove_if(slices.begin(), slices.end(),
                                    [targetWithFee](const Cells& slice) {
                                        return sum(slice) < targetWithFee;
                                    }),
                     slices.end());
        if (!slices.empty()) {
            return filterOutDust(slices.front(), sizeOfSingleInputAndWitness, byteFee);
        }
    }

    return {};
}

Cells InputSelector::selectSimple(int64_t targetValue, int64_t sizeWithoutInputs,
                                  int64_t sizeOfSingleInputAndWitness, int64_t byteFee) {
    // if target value is zero, no Cells are needed
    if (targetValue == 0) {
        return {};
    }
    if (inputs.empty()) {
        return {};
    }

    // target value is larger that original, but not by a factor of 2 (optioized for large Cell
    // cases)
    const auto increasedTargetValue =
        (int64_t)((double)targetValue * 1.1 +
                  (sizeWithoutInputs + inputs.size() * sizeOfSingleInputAndWitness) * byteFee +
                  1000);

    const int64_t dustThreshold = sizeOfSingleInputAndWitness * byteFee;

    // Go through inputs in a single pass, in the order they appear, no optimization
    int64_t sum = 0;
    Cells selected;
    for (auto& input : inputs) {
        if (input.capacity <= dustThreshold) {
            continue; // skip dust
        }
        selected.push_back(input);
        sum += input.capacity;
        if (sum >= increasedTargetValue) {
            // we have enough
            return selected;
        }
    }

    // not enough
    return {};
}

Cells InputSelector::selectMaxAmount(int64_t sizeOfSingleInputAndWitness, int64_t byteFee) {
    return filterOutDust(inputs, sizeOfSingleInputAndWitness, byteFee);
}
