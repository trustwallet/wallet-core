// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWCryptoorgCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCryptoorg));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("D87D2EB46B21466886EE149C1DEA3AE6C2E019C7D8C24FA1533A95439DDCE1E2"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCryptoorg, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("cro10wrflcdc4pys9vvgqm98yg7yv5ltj7d3xehent"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCryptoorg, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCryptoorg));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCryptoorg));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCryptoorg), 8);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCryptoorg));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCryptoorg));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCryptoorg));
    assertStringsEqual(symbol, "CRO");
    assertStringsEqual(txUrl, "https://crypto.org/explorer/tx/D87D2EB46B21466886EE149C1DEA3AE6C2E019C7D8C24FA1533A95439DDCE1E2");
    assertStringsEqual(accUrl, "https://crypto.org/explorer/account/cro10wrflcdc4pys9vvgqm98yg7yv5ltj7d3xehent");
    assertStringsEqual(id, "cryptoorg");
    assertStringsEqual(name, "Crypto.org");
}
