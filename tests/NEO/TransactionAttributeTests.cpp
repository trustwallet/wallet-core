// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "uint256.h"
#include "HexCoding.h"
#include "NEO/ReadData.h"
#include "NEO/TransactionAttribute.h"
#include "NEO/TransactionAttributeUsage.h"

#include <iostream>
#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOTransactionAttribute, Serialize) {
    auto transactionAttribute = TransactionAttribute();
    string data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_ContractHash;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("00" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4b";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Vote;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("30" + data, hex(transactionAttribute.serialize()));

    transactionAttribute.usage = TransactionAttributeUsage::TAU_ECDH02;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("02" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Script;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("20" + data, hex(transactionAttribute.serialize()));

    data = "bd";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_DescriptionUrl;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("81" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Remark;
    transactionAttribute.data = parse_hex(data);
    EXPECT_EQ("f0" + data, hex(transactionAttribute.serialize()));
}

TEST(NEOTransactionAttribute, Deserialize) {
    auto transactionAttribute = TransactionAttribute();
    string data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    transactionAttribute.deserialize(parse_hex("00" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_ContractHash, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4b";
    transactionAttribute.deserialize(parse_hex("30" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_Vote, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    transactionAttribute.deserialize(parse_hex("02" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_ECDH02, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af";
    transactionAttribute.deserialize(parse_hex("20" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_Script, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    data = "bd";
    transactionAttribute.deserialize(parse_hex("81" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_DescriptionUrl, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea";
    transactionAttribute.deserialize(parse_hex("f0" + data));
    EXPECT_EQ(TransactionAttributeUsage::TAU_Remark, transactionAttribute.usage);
    EXPECT_EQ(data, hex(transactionAttribute.data));

    EXPECT_THROW(transactionAttribute.deserialize(parse_hex("b1" + data)), std::invalid_argument);
}

TEST(NEOTransactionAttribute, DeserializeInitialPositionAfterData) {
    auto transactionAttribute = TransactionAttribute();
    EXPECT_THROW(transactionAttribute.deserialize(Data(), 1), std::invalid_argument);
    
    EXPECT_THROW(transactionAttribute.deserialize(Data({1}), 2), std::invalid_argument);
}
