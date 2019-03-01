// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWString.h>

const TWString *_Nonnull TWStringCreateWithNSString(NSString *_Nonnull other);
NSString *_Nonnull TWStringNSString(TWString *_Nonnull string);
