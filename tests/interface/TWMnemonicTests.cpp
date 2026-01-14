// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "HexCoding.h"
#include "Data.h"

#include <TrustWalletCore/Generated/TWMnemonic.h>

TEST(TWMnemonic, IsValid) {
    EXPECT_TRUE(TWMnemonicIsValid(STRING("credit expect life fade cover suit response wash pear what skull force").get()));
    EXPECT_FALSE(TWMnemonicIsValid(STRING("ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow").get())); // invalid word
}

TEST(TWMnemonic, isValidWord) {
    EXPECT_TRUE(TWMnemonicIsValidWord(STRING("credit").get()));
    EXPECT_FALSE(TWMnemonicIsValidWord(STRING("hybridous").get()));
}

TEST(TWMnemonic, suggest) {
    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWMnemonicSuggest(WRAPS(TWStringCreateWithUTF8Bytes("air")).get())).get())), "air airport");
    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWMnemonicSuggest(WRAPS(TWStringCreateWithUTF8Bytes("rob")).get())).get())), "robot robust");
}

TEST(TWMnemonic, GenerateAndCheckMnemonic) {
    // Test 128-bit strength (12 words)
    {
        auto mnemonic = WRAPS(TWMnemonicGenerate(128));
        auto mnemonicStr = std::string(TWStringUTF8Bytes(mnemonic.get()));

        // Count words
        int wordCount = 1;
        for (char c : mnemonicStr) {
            if (c == ' ') wordCount++;
        }
        EXPECT_EQ(wordCount, 12);

        // Verify it's valid
        EXPECT_TRUE(TWMnemonicIsValid(mnemonic.get()));
    }

    // Test 192-bit strength (18 words)
    {
        auto mnemonic = WRAPS(TWMnemonicGenerate(192));
        auto mnemonicStr = std::string(TWStringUTF8Bytes(mnemonic.get()));

        // Count words
        int wordCount = 1;
        for (char c : mnemonicStr) {
            if (c == ' ') wordCount++;
        }
        EXPECT_EQ(wordCount, 18);

        // Verify it's valid
        EXPECT_TRUE(TWMnemonicIsValid(mnemonic.get()));
    }

    // Test 256-bit strength (24 words)
    {
        auto mnemonic = WRAPS(TWMnemonicGenerate(256));
        auto mnemonicStr = std::string(TWStringUTF8Bytes(mnemonic.get()));

        // Count words
        int wordCount = 1;
        for (char c : mnemonicStr) {
            if (c == ' ') wordCount++;
        }
        EXPECT_EQ(wordCount, 24);

        // Verify it's valid
        EXPECT_TRUE(TWMnemonicIsValid(mnemonic.get()));
    }
}

TEST(TWMnemonic, GenerateMnemonicFromData) {
    struct TestVector {
        const char* entropyHex;
        const char* expectedMnemonic;
        const char* expectedSeed;
    };

    TestVector testVectors[] = {
        {
            "00000000000000000000000000000000",
            "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
            "c55257c360c07c72029aebc1b53c05ed0362ada38ead3e3e9efa3708e53495531f09a6987599d18264c1e1c92f2cf141630c7a3c4ab7c81b2f001698e7463b04"
        },
        {
            "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
            "legal winner thank year wave sausage worth useful legal winner thank yellow",
            "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6fa457fe1296106559a3c80937a1c1069be3a3a5bd381ee6260e8d9739fce1f607"
        },
        {
            "80808080808080808080808080808080",
            "letter advice cage absurd amount doctor acoustic avoid letter advice cage above",
            "d71de856f81a8acc65e6fc851a38d4d7ec216fd0796d0a6827a3ad6ed5511a30fa280f12eb2e47ed2ac03b5c462a0358d18d69fe4f985ec81778c1b370b652a8"
        },
        {
            "ffffffffffffffffffffffffffffffff",
            "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
            "ac27495480225222079d7be181583751e86f571027b0497b5b5d11218e0a8a13332572917f0f8e5a589620c6f15b11c61dee327651a14c34e18231052e48c069"
        },
        {
            "0000000000000000000000000000000000000000000000000000000000000000",
            "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
            "bda85446c68413707090a52022edd26a1c9462295029f2e60cd7c4f2bbd3097170af7a4d73245cafa9c3cca8d561a7c3de6f5d4a10be8ed2a5e608d68f92fcc8"
        },
        {
            "18ab19a9f54a9274f03e5209a2ac8a91",
            "board flee heavy tunnel powder denial science ski answer betray cargo cat",
            "6eff1bb21562918509c73cb990260db07c0ce34ff0e3cc4a8cb3276129fbcb300bddfe005831350efd633909f476c45c88253276d9fd0df6ef48609e8bb7dca8"
        },
        {
            "15da872c95a13dd738fbf50e427583ad61f18fd99f628c417a61cf8343c90419",
            "beyond stage sleep clip because twist token leaf atom beauty genius food business side grid unable middle armed observe pair crouch tonight away coconut",
            "b15509eaa2d09d3efd3e006ef42151b30367dc6e3aa5e44caba3fe4d3e352e65101fbdb86a96776b91946ff06f8eac594dc6ee1d3e82a42dfe1b40fef6bcc3fd"
        }
    };

    for (const auto& testVector : testVectors) {
        auto entropy = TW::parse_hex(testVector.entropyHex);
        auto entropyData = WRAPD(TWDataCreateWithBytes(entropy.data(), entropy.size()));

        auto mnemonic = WRAPS(TWMnemonicGenerateFromData(entropyData.get()));
        auto mnemonicStr = std::string(TWStringUTF8Bytes(mnemonic.get()));

        EXPECT_EQ(mnemonicStr, testVector.expectedMnemonic);

        // Verify it's valid
        EXPECT_TRUE(TWMnemonicIsValid(mnemonic.get()));

        // Generate seed with passphrase "TREZOR"
        auto passphrase = STRING("TREZOR");
        auto seed = WRAPD(TWMnemonicToSeed(mnemonic.get(), passphrase.get()));
        assertHexEqual(seed, testVector.expectedSeed);
    }
}

TEST(TWMnemonic, FindWord) {
    // Test invalid words
    EXPECT_EQ(TWMnemonicFindWord(STRING("aaaa").get()), -1);
    EXPECT_EQ(TWMnemonicFindWord(STRING("zzzz").get()), -1);

    // Test all 2048 words
    for (int32_t i = 0; i < 2048; i++) {
        auto word = WRAPS(TWMnemonicGetWord(i));
        EXPECT_NE(word.get(), nullptr);

        auto index = TWMnemonicFindWord(word.get());
        EXPECT_EQ(index, i);
    }
}

TEST(TWMnemonic, Spanish) {
    // Spanish mnemonic should not be valid in English wordlist
    auto mnemonic = STRING("llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut");

    // Should not convert to entropy (returns empty)
    auto entropy = WRAPD(TWMnemonicToEntropy(mnemonic.get()));
    EXPECT_EQ(TWDataSize(entropy.get()), 0ul);

    // But should still generate a seed (as it treats it as a passphrase)
    auto passphrase = STRING("");
    auto seed = WRAPD(TWMnemonicToSeed(mnemonic.get(), passphrase.get()));
    assertHexEqual(seed, "ec8f8703432fc7d32e699ee056e9d84b1435e6a64a6a40ad63dbde11eab189a276ddcec20f3326d3c6ee39cbd018585b104fc3633b801c011063ae4c318fb9b6");
}

