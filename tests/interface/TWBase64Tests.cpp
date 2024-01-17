// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "Data.h"
#include <TrustWalletCore/TWBase64.h>

#include <gtest/gtest.h>

TEST(TWBase64, Decode) {
    const auto encodedInput = STRING("Kyc/YWI=");
    auto result = WRAPD(TWBase64Decode(encodedInput.get()));

    ASSERT_EQ(TWDataSize(result.get()), 5ul);

    auto data = *reinterpret_cast<const TW::Data*>(result.get());
    std::string str(data.begin(), data.end());

    ASSERT_EQ(str, "+\'?ab");
}

TEST(TWBase64, Encode) {
    TW::Data data{'+', '\'', '?', 'a', 'b'};
    auto encodedStr = TWBase64Encode(&data);
    std::string result = TWStringUTF8Bytes(encodedStr);

    ASSERT_EQ(result, "Kyc/YWI=");

    TWStringDelete(encodedStr);
}

TEST(TWBase64, DecodeUrl) {
    const auto encodedInput = STRING("Kyc_YWI=");
    auto result = WRAPD(TWBase64DecodeUrl(encodedInput.get()));

    ASSERT_EQ(TWDataSize(result.get()), 5ul);

    auto data = *reinterpret_cast<const TW::Data*>(result.get());
    std::string str(data.begin(), data.end());

    ASSERT_EQ(str, "+\'?ab");
}

TEST(TWBase64, EncodeUrl) {
    TW::Data data{'+', '\'', '?', 'a', 'b'};
    auto encodedStr = TWBase64EncodeUrl(&data);
    std::string result = TWStringUTF8Bytes(encodedStr);

    ASSERT_EQ(result, "Kyc_YWI=");

    TWStringDelete(encodedStr);
}
