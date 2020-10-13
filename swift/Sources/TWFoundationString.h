// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWString.h"

/// Converts a NSString object to TWString, caller must delete it after use.
const TWString *_Nonnull TWStringCreateWithNSString(NSString *_Nonnull other);

/// Converts a TWString (will be deleted within this call) to a NSString object.
NSString *_Nonnull TWStringNSString(TWString *_Nonnull string);
