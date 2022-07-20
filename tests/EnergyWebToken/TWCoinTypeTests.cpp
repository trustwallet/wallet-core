// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWEnergyWebTokenCoinType, TWCoinType) {
    const auto coin = TWCoinTypeEnergyWebToken;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x17f8234973754e8f15048d56daceefc9bb9d2fa69c099596d30c569f29b618df"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x5bb497e8d9fe26e92dd1be01e32076c8e024d167"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "energyweb");
    assertStringsEqual(name, "Energy Web Token");
    assertStringsEqual(symbol, "EWT");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "246");
    assertStringsEqual(txUrl, "https://explorer.energyweb.org/tx/0x17f8234973754e8f15048d56daceefc9bb9d2fa69c099596d30c569f29b618df");
    assertStringsEqual(accUrl, "https://explorer.energyweb.org/address/0x5bb497e8d9fe26e92dd1be01e32076c8e024d167");
}
