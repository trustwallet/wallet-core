// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWXDCNetworkCoinType, TWCoinType) {
    const auto coin = TWCoinTypeXDCNetwork;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("xdc0b5f42A18E05d010682dc8E8CbC86cC814be4Fb5"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "xdc");
    assertStringsEqual(name, "XDC Network");
    assertStringsEqual(symbol, "XDC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "50");
    assertStringsEqual(txUrl, "https://xdc.network/tx/0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56");
    assertStringsEqual(accUrl, "https://xdc.network/address/xdc0b5f42A18E05d010682dc8E8CbC86cC814be4Fb5");
}
