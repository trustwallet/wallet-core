// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Fee.h"

#include "HexCoding.h"
#include "Data.h"
#include "uint256.h"
#include <boost/serialization/nvp.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

#include <cmath>
#include <cstddef>
#include <vector>

using json = nlohmann::json;
using float128_t = boost::multiprecision::cpp_bin_float_quad;
using namespace std;

namespace TW::Ethereum::Fee {

static const uint256_t defaultTip = 2000000000;

auto samplingCurve(double sumWeight) -> double {
    static const double sampleMin = 0.1;
    static const double sampleMax = 0.3;

    if (sumWeight <= sampleMin) {
        return 0.0;
    }

    if (sumWeight >= sampleMax) {
        return 1.0;
    }

    return (1 - cos((sumWeight - sampleMin) * 2 * M_PI / (sampleMax - sampleMin))) / 2;
}

auto suggestTip(const json& feeHistory) -> uint256_t {

    if (!feeHistory.contains("reward")) {
        return defaultTip;
    }

    vector<uint256_t> rewards;

    for (auto& item : feeHistory["reward"]) {
        const auto hex = parse_hex(item[0]);
        const auto reward = load(hex);
        if (reward > 0 ) {
            rewards.push_back(reward);
        }
    }

    if (rewards.empty()) {
        return defaultTip;
    }

    // return the median of the rewards
    sort(rewards.begin(), rewards.end());
    return rewards[rewards.size() / 2];
}

auto suggestBaseFee(vector<uint256_t> baseFees, vector<size_t> order, double timeFactor) -> uint256_t {
    if (timeFactor < 1e-6) {
        return baseFees.back();
    }

    double pendingWeight = (1 - exp(-1 / timeFactor)) / (1 - exp(-1 * double(baseFees.size()) / timeFactor));
    double sumWeight = 0.0;
    double samplingCurveLast = 0.0;
    float128_t result = 0;
    for(size_t i = 0; i < order.size(); i++) {
        sumWeight += pendingWeight * exp((double(order[i]) - baseFees.size() + 1) / timeFactor);
        double samplingCurveValue = samplingCurve(sumWeight);
        result += (samplingCurveValue - samplingCurveLast) * float128_t(baseFees[order[i]]);
        if (samplingCurveValue >= 1) {
            return uint256_t(result);
        }
        samplingCurveLast = samplingCurveValue;
    }
    return uint256_t(result);
}

auto suggestFee(const json& feeHistory) -> json {
    // tailored from: https://github.com/zsfelfoldi/ethereum-docs/blob/master/eip1559/feeHistory_example.js
    vector<uint256_t> baseFees;
    vector<size_t> order;

    const auto& array = feeHistory["baseFeePerGas"];
    for(size_t i = 0; i < array.size(); i++) {
        const auto hex = parse_hex(array[i]);
        baseFees.push_back(load(hex));
        order.push_back(i);
    }

    // The last (pending) block is also assumed to end up being full
    baseFees.back() = baseFees.back() * 9 / 8;

    const auto& gasRatio = feeHistory["gasUsedRatio"];
    for (size_t i = gasRatio.size() - 1; i > 0; i--) {
        if (i < gasRatio.size() && gasRatio[i] > 0.9) {
            baseFees[i] = baseFees[i + 1];
        }
    }

    std::sort(order.begin(), order.end(), [&baseFees] (const size_t lhs, const size_t rhs) {
        return baseFees[lhs] > baseFees[rhs];
    });

    const auto baseFee = suggestBaseFee(baseFees, order, 15);
    const auto tip = suggestTip(feeHistory);

    return json{
        {"baseFee", toString(baseFee)},
        {"maxPriorityFee", toString(tip)},
    };
}

} // namespace TW::Ethereum::Fee
