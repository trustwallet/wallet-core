// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWNapi.h"

using namespace Napi;

/// Converts a TWData to a JavaScript Uint8Array.
Uint8Array TWDataUint8Array(Env *_Nonnull env, TWData *_Nonnull data);

/// Converts a JavaScript Uint8Array to a TWData.
TWData *_Nonnull TWDataCreateWithUint8Array(Uint8Array data);
