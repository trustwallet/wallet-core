// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

    string expected =
        "{"
        R"("nonce":42,"value":"1000000000000000000",)"
        R"("receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",)"
        R"("sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",)"
        R"("gasPrice":1000000000,"gasLimit":54500,"data":"Zm9v","chainID":"1","version":2)"
        "}";

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

    string expected =
        "{"
        R"("nonce":42,"value":"1000000000000000000",)"
        R"("receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",)"
        R"("sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",)"
        R"("gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":2)"
        "}";

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

    string expected =
        "{"
        R"("nonce":42,"value":"1000000000000000000",)"
        R"("receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",)"
        R"("sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",)"
        R"("senderUsername":"YWxpY2U=","receiverUsername":"Ym9i",)"
        R"("gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2)"
        "}";

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

    string expected =
        "{"
        R"("nonce":42,"value":"1000000000000000000",)"
        R"("receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",)"
        R"("sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",)"
        R"("gasPrice":1000000000,"gasLimit":100000,"chainID":"1","version":2,"options":2,)"
        R"("guardian":"erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8")"
        "}";

    string actual = serializeTransaction(transaction);
    ASSERT_EQ(expected, actual);
}

} // namespace TW::MultiversX::tests
