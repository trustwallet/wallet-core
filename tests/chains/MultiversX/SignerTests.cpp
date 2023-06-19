// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "boost/format.hpp"
#include "HexCoding.h"
#include "MultiversX/Address.h"
#include "MultiversX/Codec.h"
#include "MultiversX/Signer.h"
#include "MultiversX/Transaction.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestAccounts.h"
#include "TestUtilities.h"

using namespace TW;

namespace TW::MultiversX::tests {

TEST(MultiversXSigner, SignGenericAction) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("foo");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(54500);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "61362540ad012ebff8436aa7fed7567639e7ef3150434b880975d844fde8cbb4e637e5537cb895ba2d0b12014ada866080b379dd96e2a7c150818a9956fb7b00";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"Zm9v",
                                     "gasLimit":54500,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"61362540ad012ebff8436aa7fed7567639e7ef3150434b880975d844fde8cbb4e637e5537cb895ba2d0b12014ada866080b379dd96e2a7c150818a9956fb7b00",
                                     "value":"0",
                                     "version":1
                                    })"_json;

    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
}

TEST(MultiversXSigner, SignGenericActionUnDelegate) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(6);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("unDelegate@" + TW::MultiversX::Codec::encodeBigInt("1000000000000000000"));
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(12000000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "89f9683af92f7b835bff4e1d0dbfcff5245b3367df4d23538eb799e0ad0a90be29ac3bd3598ce55b35b35ebef68bfa5738eed39fd01adc33476f65bd1b966e0b";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"dW5EZWxlZ2F0ZUAwZGUwYjZiM2E3NjQwMDAw",
                                     "gasLimit":12000000,
                                     "gasPrice":1000000000,
                                     "nonce":6,
                                     "receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r",
                                     "sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa",
                                     "signature":"89f9683af92f7b835bff4e1d0dbfcff5245b3367df4d23538eb799e0ad0a90be29ac3bd3598ce55b35b35ebef68bfa5738eed39fd01adc33476f65bd1b966e0b",
                                     "value":"0",
                                     "version":1
                                    })"_json;
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
    // Successfully broadcasted https://explorer.multiversx.com/transactions/3301ae5a6a77f0ab9ceb5125258f12539a113b0c6787de76a5c5867f2c515d65
}

TEST(MultiversXSigner, SignGenericActionRedelegateRewards) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("reDelegateRewards");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(12000000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "fc0238d41e4d02a24ac8a502cc3d59e406258b5c186c883e2e9aeffa859a818f5317bf22c9bc6d3838c64529953a46c1d4aabc485f96675a4c4dd642f5f50402";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"cmVEZWxlZ2F0ZVJld2FyZHM=",
                                     "gasLimit":12000000,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r",
                                     "sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa",
                                     "signature":"fc0238d41e4d02a24ac8a502cc3d59e406258b5c186c883e2e9aeffa859a818f5317bf22c9bc6d3838c64529953a46c1d4aabc485f96675a4c4dd642f5f50402",
                                     "value":"0",
                                     "version":1
                                    })"_json;
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
}

TEST(MultiversXSigner, SignGenericActionClaimRewards) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("claimRewards");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(6000000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "c453652214d428045721ad5560194a699ce4194ba7edcbdc1c4f5d1e9a605b82bb0a0fd7dba708322b62518d5d5af3e7380efab0804ac00cdafe7598e7498900";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"Y2xhaW1SZXdhcmRz",
                                     "gasLimit":6000000,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r",
                                     "sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa",
                                     "signature":"c453652214d428045721ad5560194a699ce4194ba7edcbdc1c4f5d1e9a605b82bb0a0fd7dba708322b62518d5d5af3e7380efab0804ac00cdafe7598e7498900",
                                     "value":"0",
                                     "version":1
                                    })"_json;
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
}

TEST(MultiversXSigner, SignGenericActionWithdrawStake) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("withdraw");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(12000000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "a2a17498e78e29082c433c009895bd949fc68b2222620d8f5350f821350cde390c15ffe00df4f0e84a074abd892331b79503bf458a35cb90333d1350553d9302";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"d2l0aGRyYXc=",
                                     "gasLimit":12000000,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r",
                                     "sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa",
                                     "signature":"a2a17498e78e29082c433c009895bd949fc68b2222620d8f5350f821350cde390c15ffe00df4f0e84a074abd892331b79503bf458a35cb90333d1350553d9302",
                                     "value":"0",
                                     "version":1
                                    })"_json;
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
    // Need to wait 9 days for broadcasting
}

TEST(MultiversXSigner, SignGenericActionDelegate) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(1);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("1");
    input.mutable_generic_action()->set_data("delegate");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(12000000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "3b9164d47a4e3c0330ae387cd29ba6391f9295acf5e43a16a4a2611645e66e5fa46bf22294ca68fe1948adf45cec8cb47b8792afcdb248bd9adec7c6e6c27108";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"ZGVsZWdhdGU=",
                                     "gasLimit":12000000,
                                     "gasPrice":1000000000,
                                     "nonce":1,
                                     "receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r",
                                     "sender":"erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa",
                                     "signature":"3b9164d47a4e3c0330ae387cd29ba6391f9295acf5e43a16a4a2611645e66e5fa46bf22294ca68fe1948adf45cec8cb47b8792afcdb248bd9adec7c6e6c27108",
                                     "value":"1",
                                     "version":1
                                    })"_json;
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
    ASSERT_EQ(expectedSignature, signature);
    // Successfully broadcasted https://explorer.multiversx.com/transactions/e5007662780f8ed677b37b156007c24bf60b7366000f66ec3525cfa16a4564e7
}

TEST(MultiversXSigner, SignGenericActionJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = R"(
        {
            "genericAction" : {
                "accounts": {
                    "senderNonce": 7,
                    "receiver": "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                    "sender": "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
                },
                "data": "foo",
                "value": "0",
                "version": 1
            }, 
            "gasPrice": 1000000000,
            "gasLimit": 54500,
            "chainId": "1"
        })";

    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    auto encoded = Signer::signJSON(input, privateKey.bytes);
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"Zm9v",
                                     "gasLimit":54500,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"61362540ad012ebff8436aa7fed7567639e7ef3150434b880975d844fde8cbb4e637e5537cb895ba2d0b12014ada866080b379dd96e2a7c150818a9956fb7b00",
                                     "value":"0",
                                     "version":1
                                    })"_json;

    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignWithoutData) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(0);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "c7253b821c68011584ebd3a5bb050ade19235c2d10260e411e523105826c40a79849b3eeb96fcc2a7a6b1fa140b6756f50b249e005be056ce0cf53125e0b1b00";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":50000,
                                     "gasPrice":1000000000,
                                     "nonce":0,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"c7253b821c68011584ebd3a5bb050ade19235c2d10260e411e523105826c40a79849b3eeb96fcc2a7a6b1fa140b6756f50b249e005be056ce0cf53125e0b1b00",
                                     "value":"0",
                                     "version":1
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignJSONWithoutData) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = R"(
        {
            "genericAction" : {
                "accounts": {
                    "senderNonce": 0,
                    "receiver": "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                    "sender": "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"
                }, 
                "value": "0",
                "version": 1
            },
            "gasPrice": 1000000000,
            "gasLimit": 50000,
            "chainId": "1"
        })";

    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    auto encoded = Signer::signJSON(input, privateKey.bytes);
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":50000,
                                     "gasPrice":1000000000,
                                     "nonce":0,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"c7253b821c68011584ebd3a5bb050ade19235c2d10260e411e523105826c40a79849b3eeb96fcc2a7a6b1fa140b6756f50b249e005be056ce0cf53125e0b1b00",
                                     "value":"0",
                                     "version":1
                                    })"_json;

    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignWithUsernames) {
    // https://github.com/multiversx/mx-chain-go/blob/master/examples/construction_test.go, scenario "TestConstructTransaction_Usernames".

    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(89);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_sender_username("alice");
    input.mutable_generic_action()->mutable_accounts()->set_receiver_username("bob");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "dfffb303eee7a6df0a027171feffde001637e59164a8b8c61d387da7fcefccd08d90f7b0e6fd0b4bc7357517edc5b6ea4a5088e0fb0be314e7e597e5248a8a03";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":50000,
                                     "gasPrice":1000000000,
                                     "nonce":89,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "receiverUsername": "Ym9i",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "senderUsername": "YWxpY2U=",
                                     "signature":"dfffb303eee7a6df0a027171feffde001637e59164a8b8c61d387da7fcefccd08d90f7b0e6fd0b4bc7357517edc5b6ea4a5088e0fb0be314e7e597e5248a8a03",
                                     "value":"0",
                                     "version":1
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignWithOptions) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(89);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("");
    input.mutable_generic_action()->set_version(2);
    // We'll set a dummy value on the "options" field (merely an example).
    // Currently, the "options" field should be ignored (not set) by applications using TW Core.
    input.mutable_generic_action()->set_options(42);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "ea478652417dc319c3e898d7f99f3a7b04fd32b62a7d43d5d6822a6a46b9346853426ac2ad5cdc710f0f3c5a6f509b21195e712ed9b3c95f454c7ed85079cb0b";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":50000,
                                     "gasPrice":1000000000,
                                     "nonce":89,
                                     "options": 42,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"ea478652417dc319c3e898d7f99f3a7b04fd32b62a7d43d5d6822a6a46b9346853426ac2ad5cdc710f0f3c5a6f509b21195e712ed9b3c95f454c7ed85079cb0b",
                                     "value":"0",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignEGLDTransfer) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_egld_transfer()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_egld_transfer()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_egld_transfer()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_egld_transfer()->set_amount("1000000000000000000");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "0f40dec9d37bde3c67803fc535088e536344e271807bb7c1aa24af3c69bffa9b705e149ff7bcaf21678f4900c4ee72741fa6ef08bf4c67fc6da1c6b0f337730e";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":50000,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"0f40dec9d37bde3c67803fc535088e536344e271807bb7c1aa24af3c69bffa9b705e149ff7bcaf21678f4900c4ee72741fa6ef08bf4c67fc6da1c6b0f337730e",
                                     "value":"1000000000000000000",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignESDTTransfer) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_esdt_transfer()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_esdt_transfer()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_esdt_transfer()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_esdt_transfer()->set_token_identifier("MYTOKEN-1234");
    input.mutable_esdt_transfer()->set_amount("10000000000000");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "dd7cdc90aa09da6034b00a99e3ba0f1a2a38fa788fad018d53bf2e706f99e1a42c80063c28e6b48a5f2574c4054986f34c8eb36b1da63a22d19cf3ea5990b306";

    // "ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000"
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA=",
                                     "gasLimit":425000,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"dd7cdc90aa09da6034b00a99e3ba0f1a2a38fa788fad018d53bf2e706f99e1a42c80063c28e6b48a5f2574c4054986f34c8eb36b1da63a22d19cf3ea5990b306",
                                     "value":"0",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignESDTNFTTransfer) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_esdtnft_transfer()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_esdtnft_transfer()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_esdtnft_transfer()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_esdtnft_transfer()->set_token_collection("LKMEX-aab910");
    input.mutable_esdtnft_transfer()->set_token_nonce(4);
    input.mutable_esdtnft_transfer()->set_amount("184300000000000000");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "59af89d9a9ece1f35bc34323c42061cae27bb5f9830f5eb26772e680732cbd901a86caa7c3eadacd392fe1024bef4c1f08ce1dfcafec257d6f41444ccea30a0c";

    // "ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8"
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "data":"RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg=",
                                     "gasLimit":937500,
                                     "gasPrice":1000000000,
                                     "nonce":7,
                                     "receiver":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"59af89d9a9ece1f35bc34323c42061cae27bb5f9830f5eb26772e680732cbd901a86caa7c3eadacd392fe1024bef4c1f08ce1dfcafec257d6f41444ccea30a0c",
                                     "value":"0",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignGenericActionWithGuardian) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(42);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_guardian(CAROL_BECH32);
    input.mutable_generic_action()->set_value("1000000000000000000");
    input.mutable_generic_action()->set_data("");
    input.mutable_generic_action()->set_version(2);
    input.mutable_generic_action()->set_options(TransactionOptions::Guarded);
    input.set_gas_price(1000000000);
    input.set_gas_limit(100000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "dae30e5cddb4a1f050009f939ce2c90843770870f9e6c77366be07e5cd7b3ebfdda38cd45d04e9070037d57761b6a68cee697e6043057f9dc565a4d0e632480d";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":100000,
                                     "gasPrice":1000000000,
                                     "guardian":"erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8",
                                     "nonce":42,
                                     "options":2,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"dae30e5cddb4a1f050009f939ce2c90843770870f9e6c77366be07e5cd7b3ebfdda38cd45d04e9070037d57761b6a68cee697e6043057f9dc565a4d0e632480d",
                                     "value":"1000000000000000000",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(MultiversXSigner, SignEGLDTransferWithGuardian) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_egld_transfer()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_egld_transfer()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_egld_transfer()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_egld_transfer()->mutable_accounts()->set_guardian(CAROL_BECH32);
    input.mutable_egld_transfer()->set_amount("1000000000000000000");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "741dd0d24db4df37a050f693f8481b6e51b8dd6dfc2f01a4f90aa1af3e59c89a8b0ef9d710af33103970e353d9f0cb9fd128a2e174731cbc88265d9737ed5604";
    nlohmann::json expected = R"(
                                    {
                                     "chainID":"1",
                                     "gasLimit":100000,
                                     "gasPrice":1000000000,
                                     "guardian":"erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8",
                                     "nonce":7,
                                     "options":2,
                                     "receiver":"erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx",
                                     "sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th",
                                     "signature":"741dd0d24db4df37a050f693f8481b6e51b8dd6dfc2f01a4f90aa1af3e59c89a8b0ef9d710af33103970e353d9f0cb9fd128a2e174731cbc88265d9737ed5604",
                                     "value":"1000000000000000000",
                                     "version":2
                                    })"_json;

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(ElrondSigner, buildUnsignedTxBytes) {
    auto input = Proto::SigningInput();
    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("foo");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("1");
    auto unsignedTxBytes = Signer::buildUnsignedTxBytes(input);
    auto expectedData = TW::data((boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1})") % BOB_BECH32 % ALICE_BECH32).str());
    ASSERT_EQ(expectedData, unsignedTxBytes);
}

TEST(ElrondSigner, buildSigningOutput) {
    auto input = Proto::SigningInput();
    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(7);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("foo");
    input.mutable_generic_action()->set_version(1);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("1");
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto unsignedTxBytes = Signer::buildUnsignedTxBytes(input);
    auto signature = privateKey.sign(unsignedTxBytes, TWCurveED25519);

    auto output = Signer::buildSigningOutput(input, signature);
    std::string expectedSignatureHex = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700";
    ASSERT_EQ(expectedSignatureHex, hex(signature));
    auto expectedEncoded = (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignatureHex).str();
    ASSERT_EQ(output.signature(), expectedSignatureHex);
    ASSERT_EQ(output.encoded(), expectedEncoded);
}

} // namespace TW::MultiversX::tests
