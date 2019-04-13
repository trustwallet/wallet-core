// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWNapi.h"

using namespace Napi;

/// Converts a TWString to a JavaScript string.
String TWStringJsString(Env *_Nonnull env, TWString *_Nonnull string);

/// Converts a JavaScript string to a TWString.
TWString *_Nonnull TWStringCreateWithJsString(String string);
