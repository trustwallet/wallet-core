// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Bitcoin/Entry.h"

namespace TW::BitcoinCash {

/// Entry point for BitcoinCash coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
///
/// Currently, we must support the legacy `Bitcoin.proto` API,
/// but `BitcoinV2.proto` still can be used through `Bitcoin.SigningInput.signing_v2`.
/// TODO inherit Rust::RustCoinEntry directly when `Bitcoin.proto` is deprecated.
class Entry : public Bitcoin::Entry {
};

} // namespace TW::BitcoinCash

