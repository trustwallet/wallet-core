// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWBase32.h>
#include "Data.h"

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
    ASSERT_EQ(TWDataSize(result.get()), 10);
    auto data = *reinterpret_cast<const TW::Data *>(result.get());
    std::string str(data.begin(), data.end());
    ASSERT_EQ(str, "HelloWorld");
}

TEST(TWBase32, DecodeWithAlphabet) {
    const auto encodedInput = STRING("g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i");
    const auto filecoinAlphabet = STRING("abcdefghijklmnopqrstuvwxyz234567");
    auto result = WRAPD(TWBase32DecodeWithAlphabet(encodedInput.get(), filecoinAlphabet.get()));
    ASSERT_NE(result, nullptr);
    ASSERT_EQ(TWDataSize(result.get()), 39);
    auto data = *reinterpret_cast<const TW::Data *>(result.get());
    std::string str(data.begin(), data.end());
    ASSERT_EQ(str, "7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy");
}