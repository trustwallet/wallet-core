// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Mnemonic validate / lookup functions
TW_EXPORT_STRUCT
struct TWMnemonic;

/// Determines whether a BIP39 English mnemonic phrase is valid.
///
/// \param mnemonic Non-null BIP39 english mnemonic
/// \return true if the mnemonic is valid, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWMnemonicIsValid(TWString *_Nonnull mnemonic);

/// Determines whether word is a valid BIP39 English mnemonic word.
///
/// \param word Non-null BIP39 English mnemonic word
/// \return true if the word is a valid BIP39 English mnemonic word, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWMnemonicIsValidWord(TWString *_Nonnull word);

/// Return BIP39 English words that match the given prefix. A single string is returned, with space-separated list of words.
///
/// \param prefix Non-null string prefix
/// \return Single non-null string, space-separated list of words containing BIP39 words that match the given prefix.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWMnemonicSuggest(TWString *_Nonnull prefix);

TW_EXTERN_C_END
