// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <TrustWalletCore/TWCoinType.h>
#include <string>

namespace TW {

/// Loads an address of a particular coin from raw data.
///
/// This is deprecated and here for backwards compatibility only. Addresses
/// should always be stored and transfered in their encoded string format.
std::string loadAddress(const Data& data, TWCoinType coin);

} // namespace
