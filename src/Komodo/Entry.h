// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Zcash/Entry.h"

namespace TW::Komodo {

/// Komodo entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Zcash::Entry {
};

} // namespace TW::Komodo
