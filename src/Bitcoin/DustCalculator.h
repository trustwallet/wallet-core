// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Amount.h"
#include "FeeCalculator.h"
#include "proto/Bitcoin.pb.h"

#include <memory>
#include <TrustWalletCore/TWCoinType.h>

namespace TW::Bitcoin {

/// Interface for transaction dust amount calculator.
struct DustCalculator {
    virtual ~DustCalculator() noexcept = default;

    /// Returns a Dust threshold of a transaction UTXO or output.
    virtual Amount dustAmount(Amount byteFee) noexcept = 0;
};

/// Always returns a fixed Dust amount specified in the signing request.
class FixedDustCalculator final: public DustCalculator {
public:
    explicit FixedDustCalculator(Amount fixed) noexcept;

    Amount dustAmount([[maybe_unused]] Amount byteFee) noexcept override;

private:
    Amount fixedDustAmount {0};
};

/// Legacy Dust filter implementation using [`FeeCalculator::calculateSingleInput`].
/// Depends on a coin type, sats/Byte fee.
class LegacyDustCalculator final: public DustCalculator {
public:
    explicit LegacyDustCalculator(TWCoinType coinType) noexcept;

    Amount dustAmount(Amount byteFee) noexcept override;

private:
    const FeeCalculator& feeCalculator;
};

using DustCalculatorShared = std::shared_ptr<DustCalculator>;

DustCalculatorShared getDustCalculator(const Proto::SigningInput& input);

} // namespace TW::Bitcoin
