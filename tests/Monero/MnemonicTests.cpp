// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"
#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <Monero/mnemonic.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Monero;


TEST(Mnemonic, GenerateSeed13){
    auto words = generateSeed(seed_strength_13);
    ASSERT_EQ(words.size(), 13);
}

TEST(Mnemonic, GenerateSeed25){
    auto words = generateSeed(seed_strength_25);
    ASSERT_EQ(words.size(), 25);
}

TEST(Mnemonic, GenerateSeedInvalidStrength){
    ASSERT_THROW(generateSeed(1), std::runtime_error);
    ASSERT_THROW(generateSeed(127), std::runtime_error);
    ASSERT_THROW(generateSeed(500), std::runtime_error);
    ASSERT_THROW(generateSeed(120), std::runtime_error);
    ASSERT_THROW(generateSeed(257), std::runtime_error);
}

TEST(Mnemonic, MnemonicToSeed){
    auto mnemonic = "dummy system reunion ointment null hope memoir auctions brunt input pelican perfect laptop vehicle giving custom leisure inexact oust betting sword unveil waffle when reunion";
    std::string privateKeyHex = mnemonicDecode(mnemonic);
    ASSERT_EQ(privateKeyHex, "d17f7ee37fc904cd04692a0db2a8aa003008de6865d7b0ed7c1515b9892cca03");
}

TEST(Mnemonic, MnemonicToSeedInvalidWord){
    auto mnemonic = "BADWORD system reunion ointment null hope memoir auctions brunt input pelican perfect laptop vehicle giving custom leisure inexact oust betting sword unveil waffle when reunion";
    ASSERT_THROW(mnemonicDecode(mnemonic), std::invalid_argument);
}

TEST(Mnemonic, MnemonicToSeedInvalidWordLength){
    auto mnemonic = "dummy system reunion ointment null hope memoir auctions brunt input pelican perfect laptop vehicle giving custom leisure inexact oust betting sword unveil waffle when reunion reunion";
    ASSERT_THROW(mnemonicDecode(mnemonic), std::invalid_argument);
}

// Test case from https://monero.stackexchange.com/questions/874/what-is-the-checksum-at-the-end-of-a-mnemonic-seed
TEST(Checksum, ChecksumIndexValid24){
    std::vector<std::string> words = {
            "skirting",
            "trash",
            "phase",
            "buckets",
            "apology",
            "gags",
            "sedan",
            "coffee",
            "vinegar",
            "else",
            "fifteen",
            "pitched",
            "idled",
            "gorilla",
            "siren",
            "cucumber",
            "urban",
            "junk",
            "vastness",
            "laboratory",
            "rift",
            "rhino",
            "situated",
            "taxi"
    };


    uint32_t checksum_index = getChecksumIndex(words, 3);
    ASSERT_EQ(checksum_index, 3);
}

TEST(Checksum, ChecksumIndexValid12){
    std::vector<std::string> words = {
            "skirting",
            "trash",
            "phase",
            "buckets",
            "apology",
            "gags",
            "sedan",
            "coffee",
            "vinegar",
            "else",
            "fifteen",
            "pitched",
    };


    uint32_t checksum_index = getChecksumIndex(words, 3);
    ASSERT_EQ(checksum_index, 8);
}

TEST(Checksum, ChecksumIndexInvalidLength){
    std::vector<std::string> words = {
            "skirting",
            "trash",
            "phase",
            "buckets",
            "apology",
            "gags",
            "sedan",
            "coffee",
            "vinegar",
            "else",
            "fifteen",
            "pitched",
            "idled",

    };
    ASSERT_THROW(getChecksumIndex(words, 3), std::invalid_argument);
}
