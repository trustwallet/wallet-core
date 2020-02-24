// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWData.h>

/// Converts a NSData object to TWData, caller must delete it after use.
TWData *_Nonnull TWDataCreateWithNSData(NSData *_Nonnull data);

/// Converts a TWData (will be deleted within this call) to a NSData object.
NSData *_Nonnull TWDataNSData(TWData *_Nonnull data);
