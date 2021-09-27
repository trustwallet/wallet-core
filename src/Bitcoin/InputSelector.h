// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "FeeCalculator.h"
#include <TrustWalletCore/TWCoinType.h>

#include <numeric>
#include <vector>

namespace TW::Bitcoin {

template <typename TypeWithAmount> // TypeWithAmount has to have a uint64_t amount
class InputSelector {
  public:
    /// Selects unspent transactions to use given a target transaction value, using complete logic.
    ///
    /// \returns the list of indices of selected inputs, or an empty list if there are insufficient funds.
    std::vector<TypeWithAmount> select(int64_t targetValue, int64_t byteFee, int64_t numOutputs = 2);

    /// Selects unspent transactions to use given a target transaction value;
    /// Simplified version suitable for large number of inputs
    ///
    /// \returns the list of indices of selected inputs, or an empty list if there are insufficient funds.
    std::vector<TypeWithAmount> selectSimple(int64_t targetValue, int64_t byteFee, int64_t numOutputs = 2);

    /// Selects UTXOs for max amount; select all except those which would reduce output (dust). Return indIces.
    /// One output and no change is assumed.
    std::vector<TypeWithAmount> selectMaxAmount(int64_t byteFee);

    /// Construct, using provided feeCalculator (see getFeeCalculator()).
    explicit InputSelector(const std::vector<TypeWithAmount>& inputs, const FeeCalculator& feeCalculator) : inputs(inputs), feeCalculator(feeCalculator) {}
    InputSelector(const std::vector<TypeWithAmount>& inputs) : InputSelector(inputs, getFeeCalculator(TWCoinTypeBitcoin)) {}

    /// Sum of input amounts
    static uint64_t sum(const std::vector<TypeWithAmount>& amounts);
    /// Filters out utxos that are dust
    std::vector<TypeWithAmount> filterOutDust(const std::vector<TypeWithAmount>& inputs, int64_t byteFee);
    /// Filters out inputs below (or equal) a certain threshold limit
    std::vector<TypeWithAmount> filterThreshold(const std::vector<TypeWithAmount>& inputs, uint64_t minimumAmount);

  private:
    const std::vector<TypeWithAmount> inputs;
    const FeeCalculator& feeCalculator;
};

} // namespace TW::Bitcoin
