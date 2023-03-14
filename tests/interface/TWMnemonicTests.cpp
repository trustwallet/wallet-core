// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWMnemonic.h>

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
