// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWResultVoid.h>

#include <gtest/gtest.h>

TEST(TWResultVoid, CreateOkIsSuccess) {
    const auto result = WRAP(TWResultVoid, TWResultVoidCreateOk());
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(TWResultVoidIsSuccess(result.get()));
    EXPECT_FALSE(TWResultVoidIsErr(result.get()));
}

TEST(TWResultVoid, CreateOkGetErrIsNull) {
    const auto result = WRAP(TWResultVoid, TWResultVoidCreateOk());
    const auto err = TWResultVoidGetErr(result.get());
    EXPECT_EQ(err, nullptr);
}

TEST(TWResultVoid, CreateErrorIsErr) {
    const auto msg = WRAPS(TWStringCreateWithUTF8Bytes("something went wrong"));
    const auto result = WRAP(TWResultVoid, TWResultVoidCreateError(msg.get()));
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(TWResultVoidIsErr(result.get()));
    EXPECT_FALSE(TWResultVoidIsSuccess(result.get()));
}

TEST(TWResultVoid, CreateErrorGetErrMessage) {
    const auto msg = WRAPS(TWStringCreateWithUTF8Bytes("something went wrong"));
    const auto result = WRAP(TWResultVoid, TWResultVoidCreateError(msg.get()));
    const auto err = WRAPS(TWResultVoidGetErr(result.get()));
    ASSERT_NE(err, nullptr);
    EXPECT_EQ(std::string(TWStringUTF8Bytes(err.get())), "something went wrong");
}

TEST(TWResultVoid, CreateErrorEmptyMessage) {
    const auto msg = WRAPS(TWStringCreateWithUTF8Bytes(""));
    const auto result = WRAP(TWResultVoid, TWResultVoidCreateError(msg.get()));
    EXPECT_TRUE(TWResultVoidIsErr(result.get()));
    const auto err = WRAPS(TWResultVoidGetErr(result.get()));
    ASSERT_NE(err, nullptr);
    EXPECT_EQ(std::string(TWStringUTF8Bytes(err.get())), "");
}

TEST(TWResultVoid, Delete) {
    auto* result = TWResultVoidCreateOk();
    ASSERT_NE(result, nullptr);
    TWResultVoidDelete(result);
}