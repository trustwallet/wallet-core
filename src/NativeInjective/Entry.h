// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Cosmos/Entry.h"

namespace TW::NativeInjective {

/// Entry point for implementation of NativeEvmos coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Cosmos::Entry {
};

} // namespace TW::NativeInjective
