// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DerivationPath.h"

#include <gtest/gtest.h>

namespace TW {

TEST(DerivationPath, InitWithIndices) {
    const auto path = DerivationPath(TWPurposeBIP44, TWCoinTypeEthereum, 0, 0, 0);
    ASSERT_EQ(path.indices[0], DerivationPathIndex(44, /* hardened: */true));
    ASSERT_EQ(path.indices[1], DerivationPathIndex(60, /* hardened: */true));
    ASSERT_EQ(path.indices[2], DerivationPathIndex(0, /* hardened: */true));
    ASSERT_EQ(path.indices[3], DerivationPathIndex(0, /* hardened: */false));
    ASSERT_EQ(path.indices[4], DerivationPathIndex(0, /* hardened: */false));
}

TEST(DerivationPath, InitWithString) {
    ASSERT_NO_THROW(DerivationPath("m/44'/60'/0'/0/0"));
    const auto path = DerivationPath("m/44'/60'/0'/0/0");

    ASSERT_EQ(path.indices[0], DerivationPathIndex(44, /* hardened: */true));
    ASSERT_EQ(path.indices[1], DerivationPathIndex(60, /* hardened: */true));
    ASSERT_EQ(path.indices[2], DerivationPathIndex(0, /* hardened: */true));
    ASSERT_EQ(path.indices[3], DerivationPathIndex(0, /* hardened: */false));
    ASSERT_EQ(path.indices[4], DerivationPathIndex(0, /* hardened: */false));

    ASSERT_EQ(path.purpose(), 44);
    ASSERT_EQ(path.coin(), 60);
    ASSERT_EQ(path.account(), 0);
    ASSERT_EQ(path.change(), 0);
    ASSERT_EQ(path.address(), 0);
}

TEST(DerivationPath, InitInvalid) {
    ASSERT_THROW(DerivationPath("a/b/c"), std::invalid_argument);
    ASSERT_THROW(DerivationPath("m/44'/60''/"), std::invalid_argument);
}

TEST(DerivationPath, String) {
    const auto path = DerivationPath("m/44'/60'/0'/0/0");
    ASSERT_EQ(path.string(), "m/44'/60'/0'/0/0");
}

TEST(DerivationPath, Equal) {
    const auto path1 = DerivationPath("m/44'/60'/0'/0/0");
    const auto path2 = DerivationPath("44'/60'/0'/0/0");
    ASSERT_EQ(path1, path2);
}

} // namespace
