// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "algorithm/sort_copy.h"

#include "gtest/gtest.h"

using namespace TW;

struct Amount {
    int value;
};

TEST(SortCopy, IsSorted) {
    std::vector<int> data{9, 1, 2, 4, 5};
    const auto sorted = sortCopy(data);
    std::vector<Amount> anotherData{Amount{.value = 9}, Amount{.value = 1}, Amount{.value = 0}};
    auto sortFunctor = [](auto&& lhs, auto&& rhs) { return lhs.value < rhs.value; };
    const auto anotherSorted = sortCopy(anotherData, sortFunctor);
    ASSERT_TRUE(std::is_sorted(cbegin(sorted), cend(sorted)));
    ASSERT_TRUE(std::is_sorted(cbegin(anotherSorted), cend(anotherSorted), sortFunctor));
}