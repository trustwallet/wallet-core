// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "boost/format.hpp"
#include <gtest/gtest.h>
#include <vector>

#include "HexCoding.h"
#include "MultiversX/Serialization.h"
#include "TestAccounts.h"

using namespace TW;

namespace TW::MultiversX::tests {

TEST(MultiversXSerialization, SerializeTransactionWithData) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "1000000000000000000";
    transaction.sender = ALICE_BECH32;
    transaction.receiver = BOB_BECH32;
    transaction.gasPrice = 1000000000;
    transaction.gasLimit = 54500;
    transaction.data = "foo";
    transaction.chainID = "1";
    transaction.version = 2;

    string expected = (boost::format(R"({"nonce":42,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":54500,"data":"Zm9v","chainID":"1","version":2})") % BOB_BECH32 % ALICE_BECH32).str();
    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

TEST(MultiversXSerialization, SerializeTransactionWithoutData) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "1000000000000000000";
    transaction.sender = ALICE_BECH32;
    transaction.receiver = BOB_BECH32;
    transaction.gasPrice = 1000000000;
    transaction.gasLimit = 50000;
    transaction.chainID = "1";
    transaction.version = 2;

    string expected = (boost::format(R"({"nonce":42,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":2})") % BOB_BECH32 % ALICE_BECH32).str();
    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

TEST(MultiversXSerialization, SerializeTransactionWithUsernames) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "1000000000000000000";
    transaction.sender = ALICE_BECH32;
    transaction.senderUsername = "alice";
    transaction.receiver = BOB_BECH32;
    transaction.receiverUsername = "bob";
    transaction.gasPrice = 1000000000;
    transaction.gasLimit = 100000;
    transaction.chainID = "1";
    transaction.version = 2;

    string expected = (boost::format(R"({"nonce":42,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","senderUsername":"YWxpY2U=","receiverUsername":"Ym9i","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2})") % BOB_BECH32 % ALICE_BECH32).str();
    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

TEST(MultiversXSerialization, SerializeTransactionWithGuardianAddress) {
    Transaction transaction;
    transaction.nonce = 42;
    transaction.value = "1000000000000000000";
    transaction.sender = ALICE_BECH32;
    transaction.receiver = BOB_BECH32;
    transaction.guardian = CAROL_BECH32;
    transaction.gasPrice = 1000000000;
    transaction.gasLimit = 100000;
    transaction.chainID = "1";
    transaction.version = 2;
    transaction.options = TransactionOptions::Guarded;

    string expected = (boost::format(R"({"nonce":42,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"options":2,"guardian":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % CAROL_BECH32).str();
    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

} // namespace TW::MultiversX::tests
