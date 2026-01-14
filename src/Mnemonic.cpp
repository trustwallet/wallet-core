// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Mnemonic.h"

#include "rust/Wrapper.h"

#include <algorithm>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>

namespace TW {

const int Mnemonic::SuggestMaxCount = 10;

std::string Mnemonic::generate(uint32_t strength) {
    const Rust::TWStringWrapper result = Rust::tw_mnemonic_generate(strength);
    return result.toStringOrDefault();
}

std::string Mnemonic::generateFromData(const Data& data) {
    const Rust::TWDataWrapper dataRustData = data;
    const Rust::TWStringWrapper result = Rust::tw_mnemonic_generate_from_data(dataRustData.get());
    return result.toStringOrDefault();
}

bool Mnemonic::isValid(const std::string& mnemonic) {
    const Rust::TWStringWrapper mnemonicRustStr = mnemonic;
    return Rust::tw_mnemonic_is_valid(mnemonicRustStr.get());
}

std::string Mnemonic::getWord(uint32_t index) {
    const Rust::TWStringWrapper result = Rust::tw_mnemonic_get_word(index);
    return result.toStringOrDefault();
}

bool Mnemonic::isValidWord(const std::string& word) {
    const Rust::TWStringWrapper wordRustStr = word;
    return Rust::tw_mnemonic_is_valid_word(wordRustStr.get());
}

std::string Mnemonic::suggest(const std::string& prefix) {
    const Rust::TWStringWrapper prefixRustStr = prefix;
    const Rust::TWStringWrapper result = Rust::tw_mnemonic_suggest(prefixRustStr.get());
    return result.toStringOrDefault();
}

Data Mnemonic::toSeed(const std::string& mnemonic, const std::string& passphrase) {
    const Rust::TWStringWrapper mnemonicRustStr = mnemonic;
    const Rust::TWStringWrapper passphraseRustStr = passphrase;
    const Rust::TWDataWrapper result = Rust::tw_mnemonic_to_seed(mnemonicRustStr.get(), passphraseRustStr.get());
    return result.toDataOrDefault();
}

Data Mnemonic::toEntropy(const std::string& mnemonic) {
    const Rust::TWStringWrapper mnemonicRustStr = mnemonic;
    const Rust::TWDataWrapper result = Rust::tw_mnemonic_to_entropy(mnemonicRustStr.get());
    return result.toDataOrDefault();
}

} // namespace TW
