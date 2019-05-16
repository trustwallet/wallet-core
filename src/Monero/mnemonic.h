// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

namespace TW::Monero {

    const size_t seed_strength_13 = 128;
    const size_t seed_strength_25 = 256;

    uint32_t getChecksumIndex(std::vector<std::string> words, uint32_t prefix_length);

    std::vector<std::string> generateSeed(size_t seed_strength);

    bool mnemonicFromData(const uint8_t *data, size_t len, char *mnemonic);

    bool mnemonicGenerate(int strength, char *mnemonic);

    std::string mnemonicDecode(std::string mnemonic);
}