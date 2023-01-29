// Copyright © 2017-2022 Trust Wallet.
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


TEST(TWBlocktonCoinType, TWCoinType) {
    const auto coin = TWCoinTypeBlockton;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x8542b6c2b64e39ad20d9ca442cea3c54f2541cfb4dff76be73d75ca8ba87f12e"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x2AA8CD1846042d59cDD9075A61403B4F4627d203"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "blockton");
    assertStringsEqual(name, "Blockton");
    assertStringsEqual(symbol, "BTON");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "8272");
    assertStringsEqual(txUrl, "https://blocktonscan.com/tx/0x8542b6c2b64e39ad20d9ca442cea3c54f2541cfb4dff76be73d75ca8ba87f12e");
    assertStringsEqual(accUrl, "https://blocktonscan.com/address/0x2AA8CD1846042d59cDD9075A61403B4F4627d203");
}
