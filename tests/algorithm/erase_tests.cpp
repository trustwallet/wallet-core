// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "algorithm/erase.h"

#include "gtest/gtest.h"
#include <numeric> // std::iota

TEST(Algorithm, Erase) {
    std::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    cnt.back() = '3';
    std::size_t nbElementsErased = TW::erase(cnt, '3');
    ASSERT_EQ(cnt.size(), 8);
    ASSERT_EQ(nbElementsErased, 2);
}

TEST(Algorithm, EraseIf) {
    std::vector<char> cnt(10);
    std::iota(cnt.begin(), cnt.end(), '0');
    auto erased = TW::erase_if(cnt, [](char x) { return (x - '0') % 2 == 0; });
    ASSERT_EQ(cnt.size(), 5);
    ASSERT_EQ(erased, 5);
}