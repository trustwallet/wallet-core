// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "UnspentSelector.h"

#include <algorithm>

using namespace TW;
using namespace TW::Bitcoin;

const int64_t UnspentSelector::dustThreshold = 3 * 182;

/// A selection of unspent transactions.
struct Selection {
    std::vector<Proto::UnspentTransaction> utxos;
    int64_t total;
};

// Filters utxos that are dust
std::vector<Proto::UnspentTransaction> UnspentSelector::filterDustInput(std::vector<Proto::UnspentTransaction> selectedUtxos, int64_t byteFee) {
    std::vector<Proto::UnspentTransaction> filteredUtxos;
    for(auto utxo: selectedUtxos) {
        if (utxo.amount() > singleInputFeeCalculator(byteFee)) {
            filteredUtxos.push_back(utxo);
        }
    }
    return filteredUtxos;
}

// Slice Array
// [0,1,2,3,4,5,6,7,8,9].eachSlices(3)
// >
// [[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5], [4, 5, 6], [5, 6, 7], [6, 7, 8], [7, 8, 9]]
template<typename T>
static inline auto slice(const T& elements, size_t sliceSize) {
    std::vector<std::vector<Proto::UnspentTransaction>> slices;
    for (auto i = 0; i <= elements.size() - sliceSize; i += 1) {
        slices.emplace_back();
        slices[i].reserve(sliceSize);
        for (auto j = i; j < i + sliceSize; j += 1) {
            slices[i].push_back(elements[j]);
        }
    }
    return slices;
}

template<typename T>
std::vector<Proto::UnspentTransaction> UnspentSelector::select(const T& utxos, int64_t targetValue, int64_t byteFee, int64_t numOutputs) {
    // if target value is zero, fee is zero
    if (targetValue == 0) {
        return {};
    }

    // total values of utxos should be greater than targetValue
    if (sum(utxos) < targetValue || utxos.empty()) {
        return {};
    }

    // definitions for the following caluculation
    const auto doubleTargetValue = targetValue * 2;

    // Get all possible utxo selections up to a maximum size, sort by total amount
    auto sortedUtxos = utxos;
    std::sort(sortedUtxos.begin(), sortedUtxos.end(), [](const Proto::UnspentTransaction& lhs, const Proto::UnspentTransaction& rhs) {
        return lhs.amount() < rhs.amount();
    });

    // difference from 2x targetValue
    auto distFrom2x = [doubleTargetValue](int64_t val) -> int64_t {
        if (val > doubleTargetValue)
            return val - doubleTargetValue;
        else
            return doubleTargetValue - val;
    };

    // 1. Find a combination of the fewest outputs that is
    //    (1) bigger than what we need
    //    (2) closer to 2x the amount,
    //    (3) and does not produce dust change.
    for (auto numInputs = 1; numInputs <= sortedUtxos.size(); numInputs += 1) {
        const auto fee = feeCalculator(numInputs, numOutputs, byteFee);
        const auto targetWithFeeAndDust = targetValue + fee + dustThreshold;
        auto slices = slice(sortedUtxos, numInputs);
        slices.erase(std::remove_if(slices.begin(), slices.end(), [targetWithFeeAndDust](const std::vector<Proto::UnspentTransaction>& slice) {
            return sum(slice) < targetWithFeeAndDust;
        }), slices.end());
        if (!slices.empty()) {
            std::sort(slices.begin(), slices.end(), [distFrom2x](const std::vector<Proto::UnspentTransaction>& lhs, const std::vector<Proto::UnspentTransaction>& rhs) {
                return distFrom2x(sum(lhs)) < distFrom2x(sum(rhs));
            });
            return filterDustInput(slices.front(), byteFee);
        }
    }

    // 2. If not, find a combination of outputs that may produce dust change.
    numOutputs = 1;
    for (auto numInputs = 1; numInputs <= sortedUtxos.size(); numInputs += 1) {
        const auto fee = calculateFee(numInputs, numOutputs, byteFee);
        const auto targetWithFee = targetValue + fee;
        auto slices = slice(sortedUtxos, numInputs);
        slices.erase(std::remove_if(slices.begin(), slices.end(), [targetWithFee](const std::vector<Proto::UnspentTransaction>& slice) {
            return sum(slice) < targetWithFee;
        }), slices.end());
        if (!slices.empty()) {
            return filterDustInput(slices.front(), byteFee);
        }
    }

    return {};
}

int64_t UnspentSelector::calculateFee(size_t inputs, size_t outputs, int64_t byteFee) {
    const auto txsize = ((148 * inputs) + (34 * outputs) + 10);
    return int64_t(txsize) * byteFee;
}

int64_t UnspentSelector::calculateSingleInputFee(int64_t byteFee) {
    return int64_t(148) * byteFee;
}

template std::vector<Proto::UnspentTransaction> UnspentSelector::select(const ::google::protobuf::RepeatedPtrField<Proto::UnspentTransaction>& utxos, int64_t targetValue, int64_t byteFee, int64_t numOutputs);
template std::vector<Proto::UnspentTransaction> UnspentSelector::select(const std::vector<Proto::UnspentTransaction>& utxos, int64_t targetValue, int64_t byteFee, int64_t numOutputs);
