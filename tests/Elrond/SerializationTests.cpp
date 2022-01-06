// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <vector>
#include "boost/format.hpp"

#include "HexCoding.h"
#include "Elrond/Serialization.h"
#include "TestAccounts.h"

using namespace TW;
using namespace TW::Elrond;

TEST(ElrondSerialization, SignableString) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "43";
    transaction.sender = "alice";
    transaction.receiver = "bob";
    transaction.data = "foo";
    transaction.chainID = "1";
    transaction.version = 1;

    string jsonString = serializeTransaction(transaction);
    ASSERT_EQ(R"({"nonce":42,"value":"43","receiver":"bob","sender":"alice","gasPrice":0,"gasLimit":0,"data":"Zm9v","chainID":"1","version":1})", jsonString);
}

TEST(ElrondSerialization, SignableStringWithRealData) {
    Transaction transaction;
    transaction.nonce = 15;
    transaction.value = "100";
    transaction.sender = ALICE_BECH32;
    transaction.receiver = BOB_BECH32;
    transaction.gasPrice = 1000000000;
    transaction.gasLimit = 50000;
    transaction.data = "foo";
    transaction.chainID = "1";
    transaction.version = 1;

    string expected = (boost::format(R"({"nonce":15,"value":"100","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1})") % BOB_BECH32 % ALICE_BECH32).str();
    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

TEST(ElrondSerialization, SignableStringWithoutData) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "43";
    transaction.sender = "feed";
    transaction.receiver = "abba";
    transaction.chainID = "1";
    transaction.version = 1;

    string jsonString = serializeTransaction(transaction);
    ASSERT_EQ(R"({"nonce":42,"value":"43","receiver":"abba","sender":"feed","gasPrice":0,"gasLimit":0,"chainID":"1","version":1})", jsonString);
}
