// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Cell.h"
#include <TrustWalletCore/TWCoinType.h>

#include <numeric>
#include <vector>

namespace TW::Nervos {

class InputSelector {
  public:
    /// Selects unspent transactions to use given a target transaction value, using complete logic.
    ///
    /// \returns the list of indices of selected inputs, or an empty list if there are insufficient
    /// funds.
    Cells select(int64_t targetValue, int64_t sizeWithoutInputs,
                 int64_t sizeOfSingleInputAndWitness, int64_t byteFee);

    /// Selects unspent transactions to use given a target transaction value;
    /// Simplified version suitable for large number of inputs
    ///
    /// \returns the list of indices of selected inputs, or an empty list if there are insufficient
    /// funds.
    Cells selectSimple(int64_t targetValue, int64_t sizeWithoutInputs,
                       int64_t sizeOfSingleInputAndWitness, int64_t byteFee);

    /// Selects Cells for max amount; select all except those which would reduce output (dust).
    /// Return indIces. One output and no change is assumed.
    Cells selectMaxAmount(int64_t sizeOfSingleInputAndWitness, int64_t byteFee);

    InputSelector(const Cells& inputs) : inputs(inputs) {}

    /// Sum of input amounts
    static int64_t sum(const Cells& cells);
    /// Filters out cells that are dust
    Cells filterOutDust(const Cells& inputs, int64_t sizeOfSingleInputAndWitness, int64_t byteFee);
    /// Filters out inputs below (or equal) a certain threshold limit
    Cells filterThreshold(const Cells& inputs, int64_t minimumAmount);

  private:
    const Cells inputs;
};

} // namespace TW::Nervos
