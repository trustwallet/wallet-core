// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWMnemonic.h>

namespace TW {

TEST(TWMnemonic, IsValid) {
    EXPECT_TRUE(TWMnemonicIsValid(STRING("credit expect life fade cover suit response wash pear what skull force").get()));
    EXPECT_FALSE(TWMnemonicIsValid(STRING("ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow").get())); // invalid word
}

extern const char* SpanishBip39Dictionary;

TEST(TWMnemonic, IsValidDictionary) {
    auto dictStr = STRING(SpanishBip39Dictionary);
    auto dict = WRAP(TWBip39Dictionary, TWBip39DictionaryCreate(dictStr.get()));
    ASSERT_TRUE(dict != nullptr);

    EXPECT_TRUE(TWMnemonicIsValidDictionary(STRING("careta llanto jefe tarjeta tren osadia carga alejar banda recurso aguila macho").get(), dict.get()));
    // English phrase not valid with Spanish dictionary
    EXPECT_FALSE(TWMnemonicIsValidDictionary(STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(), dict.get()));
}

TEST(TWMnemonic, isValidWord) {
    EXPECT_TRUE(TWMnemonicIsValidWord(STRING("credit").get()));
    EXPECT_FALSE(TWMnemonicIsValidWord(STRING("hybridous").get()));
}

TEST(TWMnemonic, suggest) {
    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWMnemonicSuggest(WRAPS(TWStringCreateWithUTF8Bytes("air")).get())).get())), "air airport");
    EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWMnemonicSuggest(WRAPS(TWStringCreateWithUTF8Bytes("rob")).get())).get())), "robot robust");
}

} // namespace
