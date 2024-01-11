// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../CoinEntry.h"
#include "../Cosmos/Entry.h"

namespace TW::THORChain {

/// Entry point for implementation of THORChain coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Cosmos::Entry {
};

} // namespace TW::THORChain
