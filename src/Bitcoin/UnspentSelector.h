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

class UnspentSelector {
  public:
    /// Selects unspent transactions to use given a target transaction value.
    ///
    /// \returns the list of indices of selected inputs, or an empty list if there are
    /// insufficient funds.
    std::vector<size_t> select(int64_t targetValue, int64_t byteFee, int64_t numOutputs = 2);

    /// Selects UTXOs for max amount; select all except those which would reduce output (dust). Return indIces.
    /// One output and no change is assumed.
    std::vector<size_t> selectMaxAmount(int64_t byteFee);

    /// Construct, using provided feeCalculator (see getFeeCalculator()).
    explicit UnspentSelector(const std::vector<uint64_t> inputs, const FeeCalculator& feeCalculator) : inputs(inputs), feeCalculator(feeCalculator) {}
    UnspentSelector(const std::vector<uint64_t> inputs) : UnspentSelector(inputs, getFeeCalculator(TWCoinTypeBitcoin)) {}

    static uint64_t sum(const std::vector<uint64_t>& amounts);

  private:
    const std::vector<uint64_t> inputs;
    const FeeCalculator& feeCalculator;
    int64_t sumIndices(const std::vector<size_t>& indices);
    std::vector<size_t> filterDustInput(const std::vector<size_t>& inputIndices, int64_t byteFee);
};

} // namespace TW::Bitcoin
