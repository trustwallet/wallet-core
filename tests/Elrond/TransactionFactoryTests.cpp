// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <vector>
#include "boost/format.hpp"

#include "HexCoding.h"
#include "uint256.h"
#include "Elrond/TransactionFactory.h"
#include "TestAccounts.h"

using namespace TW;
using namespace TW::Elrond;

TEST(ElrondTransactionFactory, createEGLDTransfer) {
    Address sender, receiver;
    Address::decode(ALICE_BECH32, sender);
    Address::decode(BOB_BECH32, receiver);

    NetworkConfig networkConfig;
    TransactionFactory factory(networkConfig);

    Proto::TransactionMessage message = factory.createEGLDTransfer(
        sender,
        receiver,
        uint256_t("1000000000000000000")
    );

    ASSERT_EQ(ALICE_BECH32, message.sender());
    ASSERT_EQ(BOB_BECH32, message.receiver());
    ASSERT_EQ("", message.data());
    ASSERT_EQ("1000000000000000000", message.value());
    ASSERT_EQ(50000, message.gas_limit());
    ASSERT_EQ(1000000000, message.gas_price());
    ASSERT_EQ("1", message.chain_id());
    ASSERT_EQ(1, message.version());
}

TEST(ElrondTransactionFactory, createESDTTransfer) {
    Address sender, receiver;
    Address::decode(ALICE_BECH32, sender);
    Address::decode(BOB_BECH32, receiver);

    NetworkConfig networkConfig;
    TransactionFactory factory(networkConfig);

    Proto::TransactionMessage message = factory.createESDTTransfer(sender, receiver, "MYTOKEN-1234", uint256_t("10000000000000"));

    ASSERT_EQ(ALICE_BECH32, message.sender());
    ASSERT_EQ(BOB_BECH32, message.receiver());
    ASSERT_EQ("ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000", message.data());
    ASSERT_EQ("0", message.value());
    ASSERT_EQ(425000, message.gas_limit());
    ASSERT_EQ(1000000000, message.gas_price());
    ASSERT_EQ("1", message.chain_id());
    ASSERT_EQ(1, message.version());
}

TEST(ElrondTransactionFactory, createESDTNFTTransfer) {
    Address sender, receiver;
    Address::decode(ALICE_BECH32, sender);
    Address::decode(BOB_BECH32, receiver);

    NetworkConfig networkConfig;
    TransactionFactory factory(networkConfig);

    Proto::TransactionMessage message = factory.createESDTNFTTransfer(sender, receiver, "LKMEX-aab910", 4, uint256_t("184300000000000000"));

    ASSERT_EQ(ALICE_BECH32, message.sender());
    ASSERT_EQ(ALICE_BECH32, message.receiver());
    ASSERT_EQ("ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@c70cf50b238372fffaf7b7c5723b06b57859d424a2da621bcc1b2f317543aa36", message.data());
    ASSERT_EQ("0", message.value());
    ASSERT_EQ(937500, message.gas_limit());
    ASSERT_EQ(1000000000, message.gas_price());
    ASSERT_EQ("1", message.chain_id());
    ASSERT_EQ(1, message.version());
}

TEST(ElrondTransactionFactory, createTransfersWithChangedNetworkConfig) {
    Address sender, receiver;
    Address::decode(ALICE_BECH32, sender);
    Address::decode(BOB_BECH32, receiver);

    NetworkConfig networkConfig;

    // Set dummy values:
    networkConfig.setChainId("T");
    networkConfig.setMinGasPrice(1500000000);
    networkConfig.setMinGasLimit(60000);
    networkConfig.setGasPerDataByte(2000);
    networkConfig.setGasCostESDTTransfer(300000);
    networkConfig.setGasCostESDTNFTTransfer(300000);

    TransactionFactory factory(networkConfig);

    Proto::TransactionMessage tx1 = factory.createEGLDTransfer(sender, receiver, uint256_t("0"));
    Proto::TransactionMessage tx2 = factory.createESDTTransfer(sender, receiver, "MYTOKEN-1234", uint256_t("10000000000000"));
    Proto::TransactionMessage tx3 = factory.createESDTNFTTransfer(sender, receiver, "LKMEX-aab910", 4, uint256_t("184300000000000000"));
    
    ASSERT_EQ(60000, tx1.gas_limit());
    ASSERT_EQ(1500000000, tx1.gas_price());
    ASSERT_EQ("T", tx1.chain_id());

    ASSERT_EQ(560000, tx2.gas_limit());
    ASSERT_EQ(1500000000, tx2.gas_price());
    ASSERT_EQ("T", tx2.chain_id());

    ASSERT_EQ(1110000, tx3.gas_limit());
    ASSERT_EQ(1500000000, tx3.gas_price());
    ASSERT_EQ("T", tx3.chain_id());
}
