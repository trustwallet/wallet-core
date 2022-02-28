// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWMnemonic;

/// Determines whether a BIP39 English mnemonic phrase is valid.
TW_EXPORT_STATIC_METHOD
bool TWMnemonicIsValid(TWString *_Nonnull mnemonic);

/// Determines whether word is a valid BIP39 English menemonic word.
TW_EXPORT_STATIC_METHOD
bool TWMnemonicIsValidWord(TWString *_Nonnull word);

/// Return BIP39 English words that match the given prefix. A single string is returned, with space-separated list of words.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWMnemonicSuggest(TWString *_Nonnull prefix);

TW_EXTERN_C_END
