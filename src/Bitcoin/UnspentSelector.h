// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <numeric>
#include <vector>

#include "FeeCalculator.h"
#include <TrustWalletCore/TWCoinType.h>
#include "../proto/Bitcoin.pb.h"

namespace TW::Bitcoin {

class UnspentSelector {
  public:
    /// Selects unspent transactions to use given a target transaction value.
    ///
    /// \returns the list of selected utxos or an empty list if there are
    /// insufficient funds.
    template <typename T>
    std::vector<Proto::UnspentTransaction> select(const T& utxos, int64_t targetValue,
                                                  int64_t byteFee, int64_t numOutputs = 2);

    /// Selects UTXOs for max amount; select all except those which would reduce output (dust).
    /// One output and no change is assumed.
    template <typename T>
    std::vector<Proto::UnspentTransaction> selectMaxAmount(const T& utxos, int64_t byteFee);

    /// Construct, using provided feeCalculator (see getFeeCalculator()).
    explicit UnspentSelector(const FeeCalculator& feeCalculator) : feeCalculator(feeCalculator) {}
    UnspentSelector() : UnspentSelector(getFeeCalculator(TWCoinTypeBitcoin)) {}

    template <typename T>
    static inline int64_t sum(const T& utxos) {
        int64_t sum = 0;
        for (auto& utxo : utxos) {
            sum += utxo.amount();
        }
        return sum;
    }

  private:
    const FeeCalculator& feeCalculator;
    template <typename T> std::vector<Proto::UnspentTransaction> filterDustInput(const T& selectedUtxos, int64_t byteFee);
};

} // namespace TW::Bitcoin
