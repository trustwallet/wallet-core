// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <iostream>
#include <gtest/gtest.h>

#include "ReadData.h"
#include "UInt.hpp"
#include "HexCoding.h"
#include "NEO/TransactionAttribute.hpp"
#include "NEO/TransactionAttributeUsage.h"

using namespace std;
using namespace TW;
using namespace TW::NEO;

TEST(NEOTransactionAttribute, Serialize) {
    auto transactionAttribute = TransactionAttribute();
    string data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_ContractHash;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("00" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4b";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Vote;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("30" + data, hex(transactionAttribute.serialize()));

    transactionAttribute.usage = TransactionAttributeUsage::TAU_ECDH02;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("02" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Script;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("20" + data, hex(transactionAttribute.serialize()));

    data = "bd";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_DescriptionUrl;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("81" + data, hex(transactionAttribute.serialize()));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea";
    transactionAttribute.usage = TransactionAttributeUsage::TAU_Remark;
    transactionAttribute.data = parse_hex(data);
    ASSERT_EQ("f0" + data, hex(transactionAttribute.serialize()));
}

TEST(NEOTransactionAttribute, Deserialize) {
    auto transactionAttribute = TransactionAttribute();
    string data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    transactionAttribute.deserialize(parse_hex("00" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_ContractHash, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4b";
    transactionAttribute.deserialize(parse_hex("30" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_Vote, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));

    transactionAttribute.deserialize(parse_hex("02" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_ECDH02, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af";
    transactionAttribute.deserialize(parse_hex("20" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_Script, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));

    data = "bd";
    transactionAttribute.deserialize(parse_hex("81" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_DescriptionUrl, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));

    data = "bdecbb623eee6f9ade28d5a8ff5fb3ea";
    transactionAttribute.deserialize(parse_hex("f0" + data));
    ASSERT_EQ(TransactionAttributeUsage::TAU_Remark, transactionAttribute.usage);
    ASSERT_EQ(data, hex(transactionAttribute.data));
}
