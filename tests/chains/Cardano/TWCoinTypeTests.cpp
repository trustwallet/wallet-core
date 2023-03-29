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


TEST(TWCardanoCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCardano));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("b7a6c5cadab0f64bdc89c77ee4a351463aba5c33f2cef6bbd6542a74a90a3af3"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCardano, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("DdzFFzCqrhstpwKc8WMvPwwBb5oabcTW9zc5ykA37wJR4tYQucvsR9dXb2kEGNXkFJz2PtrpzfRiZkx8R1iNo8NYqdsukVmv7EAybFwC"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCardano, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCardano));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCardano));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCardano), 6);
    ASSERT_EQ(TWBlockchainCardano, TWCoinTypeBlockchain(TWCoinTypeCardano));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCardano));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCardano));
    assertStringsEqual(symbol, "ADA");
    assertStringsEqual(txUrl, "https://cardanoscan.io/transaction/b7a6c5cadab0f64bdc89c77ee4a351463aba5c33f2cef6bbd6542a74a90a3af3");
    assertStringsEqual(accUrl, "https://cardanoscan.io/address/DdzFFzCqrhstpwKc8WMvPwwBb5oabcTW9zc5ykA37wJR4tYQucvsR9dXb2kEGNXkFJz2PtrpzfRiZkx8R1iNo8NYqdsukVmv7EAybFwC");
    assertStringsEqual(id, "cardano");
    assertStringsEqual(name, "Cardano");
}
