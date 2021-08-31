// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWBip39Dictionary;

// Creates a new Bip39Dictionary from a single string with 2048 space-separated words
// Returned object needs to be deleted.
TW_EXPORT_STATIC_METHOD
struct TWBip39Dictionary *_Nullable TWBip39DictionaryCreate(TWString *_Nonnull dictionaryString);

TW_EXPORT_METHOD
void TWBip39DictionaryDelete(struct TWBip39Dictionary *_Nonnull dictionary);

TW_EXTERN_C_END
