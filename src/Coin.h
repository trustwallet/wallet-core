// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "DerivationPath.h"
#include "PrivateKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWCurve.h>
#include <string>

namespace TW {

/// Loads an address of a particular coin from raw data.
///
/// This is deprecated and here for backwards compatibility only. Addresses
/// should always be stored and transfered in their encoded string format.
std::string loadAddress(TWCoinType coin, const Data& data);

/// Validates an address for a particular coin.
bool validateAddress(TWCoinType coin, const std::string& address);

/// Returns the purpose for a coin type.
TWPurpose purpose(TWCoinType coin);

/// Returns the curve that should be used for a coin type.
TWCurve curve(TWCoinType coin);

/// Returns the default derivation path for a particular coin.
DerivationPath derivationPath(TWCoinType coin);

/// Derives the address for a particular coin from the private key.
std::string deriveAddress(TWCoinType coin, const PrivateKey& privateKey);

} // namespace
