// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWDataVector.h>
#include "HexCoding.h"
#include "TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;


TEST(TWDataVector, CreateDelete) {
    auto vec = TWDataVectorCreate();
    
    ASSERT_TRUE(vec != nullptr);
    EXPECT_EQ(TWDataVectorSize(vec), 0);
    
    TWDataVectorDelete(vec);
}

TEST(TWDataVector, CreateWrapAutoDelete) {
    auto vec = WRAP(TWDataVector, TWDataVectorCreate());
    
    ASSERT_TRUE(vec.get() != nullptr);
    EXPECT_EQ(TWDataVectorSize(vec.get()), 0);
}

TEST(TWDataVector, CreateWithData) {
    const auto elem1d = parse_hex("deadbeef");
    const auto elem1 = WRAPD(TWDataCreateWithBytes(elem1d.data(), elem1d.size()));
    const auto vec = WRAP(TWDataVector, TWDataVectorCreateWithData(elem1.get()));

    ASSERT_TRUE(vec.get() != nullptr);
    ASSERT_EQ(TWDataVectorSize(vec.get()), 1);

    const auto readElem1 = WRAPD(TWDataVectorGet(vec.get(), 0));
    EXPECT_EQ(hex(*static_cast<const Data*>(readElem1.get())), "deadbeef");
}

TEST(TWDataVector, Add) {
    const auto vec = WRAP(TWDataVector, TWDataVectorCreate());
    
    ASSERT_TRUE(vec.get() != nullptr);
    EXPECT_EQ(TWDataVectorSize(vec.get()), 0);

    const auto elem1d = parse_hex("deadbeef");
    const auto elem1 = WRAPD(TWDataCreateWithBytes(elem1d.data(), elem1d.size()));
    TWDataVectorAdd(vec.get(), elem1.get());

    ASSERT_EQ(TWDataVectorSize(vec.get()), 1);
    const auto readElem1 = WRAPD(TWDataVectorGet(vec.get(), 0));
    EXPECT_EQ(hex(*static_cast<const Data*>(readElem1.get())), "deadbeef");

    const auto elem2d = parse_hex("0202");
    const auto elem2 = WRAPD(TWDataCreateWithBytes(elem2d.data(), elem2d.size()));
    TWDataVectorAdd(vec.get(), elem2.get());

    ASSERT_EQ(TWDataVectorSize(vec.get()), 2);
    const auto readElem2 = WRAPD(TWDataVectorGet(vec.get(), 1));
    EXPECT_EQ(hex(*static_cast<const Data*>(readElem2.get())), "0202");
}

TEST(TWDataVector, Get) {
    const auto elem1d = parse_hex("deadbeef");
    const auto elem1 = WRAPD(TWDataCreateWithBytes(elem1d.data(), elem1d.size()));
    const auto vec = WRAP(TWDataVector, TWDataVectorCreateWithData(elem1.get()));

    ASSERT_TRUE(vec.get() != nullptr);
    ASSERT_EQ(TWDataVectorSize(vec.get()), 1);

    {   // Get element
        const auto readElem1 = WRAPD(TWDataVectorGet(vec.get(), 0));
        EXPECT_EQ(hex(*static_cast<const Data*>(readElem1.get())), "deadbeef");
    }
    {   // Get with bad index
        const auto readElem = TWDataVectorGet(vec.get(), 666);
        EXPECT_EQ(readElem, nullptr);
    }
}
