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


TEST(TWBandChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBandChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("473264551D3063A9EC64EC251C61BE92DDDFCF6CC46D026D1E574D83D5447173"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBandChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("band12nmsm9khdsv0tywge43q3zwj8kkj3hvup9rltp"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBandChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBandChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBandChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBandChain), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeBandChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBandChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBandChain));
    assertStringsEqual(symbol, "BAND");
    assertStringsEqual(txUrl, "https://scan-wenchang-testnet2.bandchain.org//tx/473264551D3063A9EC64EC251C61BE92DDDFCF6CC46D026D1E574D83D5447173");
    assertStringsEqual(accUrl, "https://scan-wenchang-testnet2.bandchain.org//account/band12nmsm9khdsv0tywge43q3zwj8kkj3hvup9rltp");
    assertStringsEqual(id, "band");
    assertStringsEqual(name, "BandChain");
}
