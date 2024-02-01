// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Mnemonic.h"

#include <TrezorCrypto/bip39_english.h>
#include <TrezorCrypto/bip39.h>

#include <algorithm>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>

namespace TW {

const int Mnemonic::SuggestMaxCount = 10;

bool Mnemonic::isValid(const std::string& mnemonic) {
    return mnemonic_check(mnemonic.c_str()) != 0;
}

inline const char* const* mnemonicWordlist() { return wordlist; }

bool Mnemonic::isValidWord(const std::string& word) {
    const char* wordC = word.c_str();
    const auto len = word.length();
    // Although this operation is not security-critical, we aim for constant-time operation here as well
    // (i.e., no early exit on match)
    auto found = false;
    for (const char* const* w = mnemonicWordlist(); *w != nullptr; ++w) {
        if (std::string(*w).size() == len && strncmp(*w, wordC, len) == 0) {
            found = true;
        }
    }
    return found;
}

std::string Mnemonic::suggest(const std::string& prefix) {
    if (prefix.size() == 0) {
        return "";
    }
    assert(prefix.size() >= 1);
    // lowercase prefix
    std::string prefixLo = prefix;
    std::transform(prefixLo.begin(), prefixLo.end(), prefixLo.begin(),
        [](unsigned char c){ return std::tolower(c); });
    const char* prefixLoC = prefixLo.c_str();

    std::vector<std::string> result;
    for (const char* const* word = mnemonicWordlist(); *word != nullptr; ++word) {
        // check first letter match (optimization)
        if ((*word)[0] == prefixLo[0]) {
            if (strncmp(*word, prefixLoC, prefixLo.length()) == 0) {
                // we have a match
                result.emplace_back(*word);
                if (result.size() >= SuggestMaxCount) {
                    break; // enough results
                }
            }
        }
    }

    // convert results to one string
    std::string resultString;
    for (auto& word: result) {
        if (resultString.length() > 0) {
            resultString += " ";
        }
        resultString += word;
    }
    return resultString;
}

} // namespace TW
