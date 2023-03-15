// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Data.h"
#include <TrustWalletCore/TWBase32.h>

#include <gtest/gtest.h>

TEST(TWBase32, InvalidDecode) {
    const auto encodedInput = STRING("JBSWY3DPK5XXE3DE=======");
    auto result = WRAPD(TWBase32Decode(encodedInput.get()));
    ASSERT_EQ(result, nullptr);
}

TEST(TWBase32, Decode) {
    const auto encodedInput = STRING("JBSWY3DPK5XXE3DE");
    auto result = WRAPD(TWBase32Decode(encodedInput.get()));

    ASSERT_NE(result, nullptr);
    ASSERT_EQ(TWDataSize(result.get()), 10ul);

    auto data = *reinterpret_cast<const TW::Data*>(result.get());
    std::string str(data.begin(), data.end());

    ASSERT_EQ(str, "HelloWorld");
}

TEST(TWBase32, DecodeWithAlphabet) {
    const auto encodedInput = STRING("g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i");
    const auto filecoinAlphabet = STRING("abcdefghijklmnopqrstuvwxyz234567");
    auto result = WRAPD(TWBase32DecodeWithAlphabet(encodedInput.get(), filecoinAlphabet.get()));

    ASSERT_NE(result, nullptr);
    ASSERT_EQ(TWDataSize(result.get()), 39ul);

    auto data = *reinterpret_cast<const TW::Data*>(result.get());
    std::string str(data.begin(), data.end());

    ASSERT_EQ(str, "7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy");
}

TEST(TWBase32, Encode) {
    TW::Data data{'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};
    auto encodedStr = TWBase32Encode(&data);
    std::string result = TWStringUTF8Bytes(encodedStr);

    ASSERT_EQ(result, "JBSWY3DPK5XXE3DE");

    TWStringDelete(encodedStr);
}

TEST(TWBase32, EncodeWithAlphabet) {
    const auto filecoinAlphabet = STRING("abcdefghijklmnopqrstuvwxyz234567");
    TW::Data data{'7', 'u', 'o', 'q', '6', 't', 'p', '4', '2', '7', 'u', 'z', 'v', '7', 'f',
                  'z', 't', 'k', 'b', 's', 'n', 'n', '6', '4', 'i', 'w', 'o', 't', 'f', 'r', 'r', 'i', 's', 't', 'w', 'p', 'r', 'y', 'y'};
    auto encodedStr = TWBase32EncodeWithAlphabet(&data, filecoinAlphabet.get());
    std::string result = TWStringUTF8Bytes(encodedStr);

    ASSERT_EQ(result, "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i");

    TWStringDelete(encodedStr);
}
