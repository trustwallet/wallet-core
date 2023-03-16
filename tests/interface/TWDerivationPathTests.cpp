// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include "TrustWalletCore/TWDerivation.h"
#include "TrustWalletCore/TWPurpose.h"
#include <TrustWalletCore/TWDerivationPath.h>
#include <TrustWalletCore/TWDerivationPathIndex.h>

#include <gtest/gtest.h>

TEST(TWDerivationPath, CreateWithString) {
    const auto derivationPath = STRING("m/84'/1'/2'/3/4");

    const auto path = WRAP(TWDerivationPath, TWDerivationPathCreateWithString(derivationPath.get()));

    EXPECT_EQ(5u, TWDerivationPathIndicesCount(path.get()));
    EXPECT_EQ(TWPurposeBIP84, TWDerivationPathPurpose(path.get()));
    EXPECT_EQ(1u, TWDerivationPathCoin(path.get()));
    EXPECT_EQ(2u, TWDerivationPathAccount(path.get()));
    EXPECT_EQ(3u, TWDerivationPathChange(path.get()));
    EXPECT_EQ(4u, TWDerivationPathAddress(path.get()));
    assertStringsEqual(WRAPS(TWDerivationPathDescription(path.get())), "m/84'/1'/2'/3/4");

    const auto index0 = WRAP(TWDerivationPathIndex, TWDerivationPathIndexAt(path.get(), 0));
    const auto index3 = WRAP(TWDerivationPathIndex, TWDerivationPathIndexAt(path.get(), 3));

    EXPECT_EQ(NULL, TWDerivationPathIndexAt(path.get(), 10));
    EXPECT_EQ(TWPurposeBIP84, TWDerivationPathIndexValue(index0.get()));
    EXPECT_TRUE(TWDerivationPathIndexHardened(index0.get()));

    EXPECT_EQ(3u, TWDerivationPathIndexValue(index3.get()));
    EXPECT_FALSE(TWDerivationPathIndexHardened(index3.get()));
    assertStringsEqual(WRAPS(TWDerivationPathIndexDescription(index0.get())), "84'");

    const auto path2 = WRAP(TWDerivationPath, TWDerivationPathCreateWithString(STRING("m/44'/501'").get()));

    EXPECT_EQ(2u, TWDerivationPathIndicesCount(path2.get()));
    EXPECT_EQ(TWPurposeBIP44, TWDerivationPathPurpose(path2.get()));
    EXPECT_EQ(501u, TWDerivationPathCoin(path2.get()));
    EXPECT_EQ(0u, TWDerivationPathAccount(path2.get()));
    EXPECT_EQ(0u, TWDerivationPathChange(path2.get()));
    EXPECT_EQ(0u, TWDerivationPathAddress(path2.get()));
}

TEST(TWDerivationPath, CreateWithCoin) {

    const auto path = WRAP(TWDerivationPath, TWDerivationPathCreate(TWPurposeBIP44, 60, 0, 0, 0));

    EXPECT_EQ(5u, TWDerivationPathIndicesCount(path.get()));
    EXPECT_EQ(TWPurposeBIP44, TWDerivationPathPurpose(path.get()));
    EXPECT_EQ(60u, TWDerivationPathCoin(path.get()));
    EXPECT_EQ(0u, TWDerivationPathAccount(path.get()));
    EXPECT_EQ(0u, TWDerivationPathChange(path.get()));
    EXPECT_EQ(0u, TWDerivationPathAddress(path.get()));
    assertStringsEqual(WRAPS(TWDerivationPathDescription(path.get())), "m/44'/60'/0'/0/0");

    const auto index = WRAP(TWDerivationPathIndex, TWDerivationPathIndexCreate(44, true));
    const auto description = WRAPS(TWDerivationPathIndexDescription(index.get()));
    assertStringsEqual(description, "44'");
}
