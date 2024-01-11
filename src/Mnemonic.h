// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>

namespace TW {

/// BIP39 Mnemonic recovery phrase handling.
class Mnemonic {
public:
    static constexpr int MaxWords = 24;
    static constexpr int MinWords = 12;
    static constexpr int BitsPerWord = 11; // each word encodes this many bits (there are 2^11=2048 different words)

public:
    /// Determines whether a BIP39 English mnemonic phrase is valid.
    // E.g. for a valid mnemonic: "credit expect life fade cover suit response wash pear what skull force"
    static bool isValid(const std::string& mnemonic);

    /// Determines whether word is a valid BIP39 English menemonic word.
    static bool isValidWord(const std::string& word);

    /// Return BIP39 English words that match the given prefix.
    // - A single string is returned, with space-separated list of words (or single word or empty string)
    //   (Why not array?  To simplify the cross-language interfaces)
    // - A maximum of 10 words are returned
    // - Only exact prefix match, saus returns sausage, but saos does not.
    // - Only word start match, rob returns robot, but not aerobic
    // - Prefix is case insensitive
    // - Returned words are in lowercase, and in alphabetical order
    // Examples:
    // - 'rob' -> 'robot robust'
    // - 'ai' -> 'aim air airport aisle'
    // - 'an' -> 'analyst anchor ancient anger angle angry animal ankle announce annual'
    // - 'a'-> 'abandon ability able about above absent absorb abstract absurd abuse'
    static std::string suggest(const std::string& prefix);

    static const int SuggestMaxCount;
};

} // namespace TW
