// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "FeeCalculator.h"
#include "DustCalculator.h"
#include <TrustWalletCore/TWCoinType.h>

#include <numeric>
#include <vector>

namespace TW::Bitcoin {

template <typename TypeWithAmount> // TypeWithAmount has to have an uint64_t amount
class InputSelector {
public:
    /// Selects unspent transactions to use given a target transaction value, using complete logic.
    ///
    /// \returns the list of indices of selected inputs. May return the entire list of UTXOs
    ///          even if they aren't enough to cover `targetValue + fee`.
    ///          That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    std::vector<TypeWithAmount> select(uint64_t targetValue, uint64_t byteFee,
                                       uint64_t numOutputs = 2);

    /// Selects unspent transactions to use given a target transaction value;
    /// Simplified version suitable for large number of inputs
    ///
    /// \returns the list of indices of selected inputs. May return the entire list of UTXOs
    ///          even if they aren't enough to cover `targetValue + fee`.
    ///          That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    std::vector<TypeWithAmount> selectSimple(int64_t targetValue, int64_t byteFee,
                                             int64_t numOutputs = 2);

    /// Selects UTXOs for max amount; select all except those which would reduce output (dust).
    /// Return indices. One output and no change is assumed.
    std::vector<TypeWithAmount> selectMaxAmount(int64_t byteFee) noexcept;

    /// Construct, using provided feeCalculator (see getFeeCalculator()) and dustCalculator (see getDustCalculator()).
    explicit InputSelector(const std::vector<TypeWithAmount>& inputs,
                           const FeeCalculator& feeCalculator,
                           DustCalculatorShared dustCalculator) noexcept
        : _inputs(inputs),
          feeCalculator(feeCalculator),
          dustCalculator(std::move(dustCalculator)) {
    }

    explicit InputSelector(const std::vector<TypeWithAmount>& inputs) noexcept
        : _inputs(inputs),
          feeCalculator(getFeeCalculator(TWCoinTypeBitcoin)),
          dustCalculator(std::make_shared<LegacyDustCalculator>(TWCoinTypeBitcoin)) {
    }

    /// Sum of input amounts
    static uint64_t sum(const std::vector<TypeWithAmount>& amounts) noexcept;
    /// Filters out utxos that are dust
    inline std::vector<TypeWithAmount> filterOutDust(const std::vector<TypeWithAmount>& inputsIn,
                                                     int64_t byteFee) noexcept;
    /// Filters out inputsIn below (or equal) a certain threshold limit
    inline std::vector<TypeWithAmount> filterThreshold(const std::vector<TypeWithAmount>& inputsIn,
                                                       uint64_t minimumAmount) noexcept;

private:
    const std::vector<TypeWithAmount> _inputs;
    const FeeCalculator& feeCalculator;
    const DustCalculatorShared dustCalculator;
};

} // namespace TW::Bitcoin
