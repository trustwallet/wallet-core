// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWTerraCoinType, TWCoinTypeClassic) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTerra));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("D28D8AFC7CE89F2A22FA2DBF78D2C0A36E549BB830C4D9FA7459E3F723CA7182"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTerra, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("terra16t3gx5rqvz6ru37yzn3shuu20erv4ngmfr59zf"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTerra, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTerra));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTerra));
    const auto chainId = WRAPS(TWCoinTypeChainId(TWCoinTypeTerra));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTerra), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeTerra));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTerra));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTerra));
    assertStringsEqual(chainId, "columbus-5");
    assertStringsEqual(symbol, "LUNC");
    assertStringsEqual(txUrl, "https://finder.terra.money/classic/tx/D28D8AFC7CE89F2A22FA2DBF78D2C0A36E549BB830C4D9FA7459E3F723CA7182");
    assertStringsEqual(accUrl, "https://finder.terra.money/classic/address/terra16t3gx5rqvz6ru37yzn3shuu20erv4ngmfr59zf");
    assertStringsEqual(id, "terra");
    assertStringsEqual(name, "Terra Classic");
}

} // namespace TW::Cosmos::tests
