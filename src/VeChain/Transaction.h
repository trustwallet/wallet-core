// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Clause.h"
#include "Data.h"

#include <cstdint>
#include <vector>

namespace TW::VeChain {

class Transaction {
  public:
    /// Last byte of the genesis block ID which is used to identify a blockchain
    /// to prevent the cross-chain replay attack.
    std::uint8_t chainTag;

    /// Reference to a specific block.
    std::uint64_t blockRef;

    /// How long, in terms of the number of blocks, the transaction will be
    /// allowed to be mined in VeChainThor.
    std::uint32_t expiration;

    /// An array of Clause objects.
    ///
    /// Each clause contains fields To, Value and Data to enable a single
    /// transaction to carry multiple tasks issued by the transaction sender.
    std::vector<Clause> clauses;

    /// Coefficient used to calculate the gas price for the transaction.
    std::uint8_t gasPriceCoef;

    /// Maximum amount of gas allowed to pay for the transaction.
    std::uint64_t gas;

    /// ID of the transaction on which the current transaction depends.
    Data dependsOn;

    /// Number set by user.
    std::uint64_t nonce;

    /// Reserved field for backward compatibility. It MUST be set empty for now
    /// otherwise the transaction will be considered invalid.
    std::vector<Data> reserved;

    /// Transaction signature.
    Data signature;

    Transaction() = default;

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

} // namespace TW::VeChain
