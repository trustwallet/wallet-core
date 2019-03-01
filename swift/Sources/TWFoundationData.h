// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWData.h>

TWData *_Nonnull TWDataCreateWithNSData(NSData *_Nonnull data);
NSData *_Nonnull TWDataNSData(TWData *_Nonnull data);
