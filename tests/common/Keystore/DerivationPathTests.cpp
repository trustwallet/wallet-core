// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "DerivationPath.h"
#include <TrustWalletCore/TWDerivation.h>

#include <gtest/gtest.h>

namespace TW {

TEST(DerivationPath, InitWithIndices) {
    const auto path = DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeEthereum), 0, 0, 0);
    ASSERT_EQ(path.indices[0], DerivationPathIndex(44, /* hardened: */ true));
    ASSERT_EQ(path.indices[1], DerivationPathIndex(60, /* hardened: */ true));
    ASSERT_EQ(path.indices[2], DerivationPathIndex(0, /* hardened: */ true));
    ASSERT_EQ(path.indices[3], DerivationPathIndex(0, /* hardened: */ false));
    ASSERT_EQ(path.indices[4], DerivationPathIndex(0, /* hardened: */ false));
}

TEST(DerivationPath, InitWithString) {
    ASSERT_NO_THROW(DerivationPath("m/44'/60'/0'/0/0"));
    const auto path = DerivationPath("m/44'/60'/0'/0/0");

    ASSERT_EQ(path.indices[0], DerivationPathIndex(44, /* hardened: */ true));
    ASSERT_EQ(path.indices[1], DerivationPathIndex(60, /* hardened: */ true));
    ASSERT_EQ(path.indices[2], DerivationPathIndex(0, /* hardened: */ true));
    ASSERT_EQ(path.indices[3], DerivationPathIndex(0, /* hardened: */ false));
    ASSERT_EQ(path.indices[4], DerivationPathIndex(0, /* hardened: */ false));

    ASSERT_EQ(path.purpose(), 44);
    ASSERT_EQ(path.coin(), 60ul);
    ASSERT_EQ(path.account(), 0ul);
    ASSERT_EQ(path.change(), 0ul);
    ASSERT_EQ(path.address(), 0ul);
}

TEST(DerivationPath, InitInvalid) {
    ASSERT_THROW(DerivationPath("a/b/c"), std::invalid_argument);
    ASSERT_THROW(DerivationPath("m/44'/60''/"), std::invalid_argument);
}

TEST(DerivationPath, IndexOutOfBounds) {
    DerivationPath path;

    EXPECT_EQ(path.indices.size(), 0ul);

    EXPECT_EQ(path.purpose(), TWPurposeBIP44);
    EXPECT_EQ(path.coin(), TWCoinTypeBitcoin);
    EXPECT_EQ(path.account(), 0ul);
    EXPECT_EQ(path.change(), 0ul);
    EXPECT_EQ(path.address(), 0ul);

    ASSERT_NO_THROW(path.setPurpose(TWPurposeBIP44));
    ASSERT_NO_THROW(path.setCoin(TWCoinTypeBitcoin));
    ASSERT_NO_THROW(path.setAccount(0));
    ASSERT_NO_THROW(path.setChange(0));
    ASSERT_NO_THROW(path.setAddress(0));
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

TEST(Derivation, derivationPath) {
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin).string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeSolana).string(), "m/44'/501'/0'");
}

TEST(Derivation, alternativeDerivation) {
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin).string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin, TWDerivationDefault).string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin, TWDerivationBitcoinSegwit).string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypeBitcoin, TWDerivationBitcoinSegwit)), "segwit");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin, TWDerivationBitcoinLegacy).string(), "m/44'/0'/0'/0/0");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypeBitcoin, TWDerivationBitcoinLegacy)), "legacy");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeBitcoin, TWDerivationBitcoinTaproot).string(), "m/86'/0'/0'/0/0");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypeBitcoin, TWDerivationBitcoinTaproot)), "taproot");

    EXPECT_EQ(TW::derivationPath(TWCoinTypeLitecoin, TWDerivationDefault).string(), "m/84'/2'/0'/0/0");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeLitecoin, TWDerivationLitecoinLegacy).string(), "m/44'/2'/0'/0/0");

    EXPECT_EQ(TW::derivationPath(TWCoinTypeSolana).string(), "m/44'/501'/0'");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeSolana, TWDerivationDefault).string(), "m/44'/501'/0'");
    EXPECT_EQ(TW::derivationPath(TWCoinTypeSolana, TWDerivationSolanaSolana).string(), "m/44'/501'/0'/0'");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypeSolana, TWDerivationSolanaSolana)), "solana");

    EXPECT_EQ(TW::derivationPath(TWCoinTypePactus).string(), "m/44'/21888'/3'/0'");
    EXPECT_EQ(TW::derivationPath(TWCoinTypePactus, TWDerivationPactusMainnet).string(), "m/44'/21888'/3'/0'");
    EXPECT_EQ(TW::derivationPath(TWCoinTypePactus, TWDerivationPactusTestnet).string(), "m/44'/21777'/3'/0'");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypePactus, TWDerivationPactusMainnet)), "mainnet");
    EXPECT_EQ(std::string(TW::derivationName(TWCoinTypePactus, TWDerivationPactusTestnet)), "testnet");
}

} // namespace TW
