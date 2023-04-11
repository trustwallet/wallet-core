// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "boost/format.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Elrond/Address.h"
#include "Elrond/Signer.h"
#include "Elrond/Codec.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestAccounts.h"
#include "TestUtilities.h"

using namespace TW;

namespace TW::Elrond::tests {

TEST(ElrondSigner, SignGenericAction) {
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
    input.set_gas_limit(50000);
    input.set_chain_id("1");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700";
    auto expectedEncoded = (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
    ASSERT_EQ(expectedSignature, signature);
}

TEST(ElrondSigner, SignGenericActionUnDelegate) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(6);
    input.mutable_generic_action()->mutable_accounts()->set_sender("erd1aajqh5xjka5fk0c235dwy7qd6lkz2e29tlhy8gncuq0mcr68q34qgswnqa");
    input.mutable_generic_action()->mutable_accounts()->set_receiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqfhllllscrt56r");
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("unDelegate@" + TW::Elrond::Codec::encodeBigInt("1000000000000000000"));
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
    // Successfully broadcasted https://explorer.elrond.com/transactions/3301ae5a6a77f0ab9ceb5125258f12539a113b0c6787de76a5c5867f2c515d65
}

TEST(ElrondSigner, SignGenericActionRedelegateRewards) {
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

TEST(ElrondSigner, SignGenericActionClaimRewards) {
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

TEST(ElrondSigner, SignGenericActionWithdrawStake) {
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

TEST(ElrondSigner, SignGenericActionDelegate) {
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
    // Successfully broadcasted https://explorer.elrond.com/transactions/e5007662780f8ed677b37b156007c24bf60b7366000f66ec3525cfa16a4564e7
}

TEST(ElrondSigner, SignGenericActionJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = (boost::format(R"({"genericAction" : {"accounts": {"senderNonce": 7, "receiver": "%1%", "sender": "%2%"}, "data": "foo", "value": "0", "version": 1}, "gasPrice": 1000000000, "gasLimit": 50000, "chainId": "1"})") % BOB_BECH32 % ALICE_BECH32).str();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));

    auto encoded = Signer::signJSON(input, privateKey.bytes);
    auto expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700";
    auto expectedEncoded = (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignWithoutData) {
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
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignJSONWithoutData) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = (boost::format(R"({"genericAction" : {"accounts": {"senderNonce": 0, "receiver": "%1%", "sender": "%2%"}, "value": "0", "version": 1}, "gasPrice": 1000000000, "gasLimit": 50000, "chainId": "1"})") % BOB_BECH32 % ALICE_BECH32).str();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));

    auto encoded = Signer::signJSON(input, privateKey.bytes);
    auto expectedSignature = "c7253b821c68011584ebd3a5bb050ade19235c2d10260e411e523105826c40a79849b3eeb96fcc2a7a6b1fa140b6756f50b249e005be056ce0cf53125e0b1b00";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignWithUsernames) {
    // https://github.com/ElrondNetwork/elrond-go/blob/master/examples/construction_test.go, scenario "TestConstructTransaction_Usernames".

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
    input.set_chain_id("local-testnet");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "1bed82c3f91c9d24f3a163e7b93a47453d70e8743201fe7d3656c0214569566a76503ef0968279ac942ca43b9c930bd26638dfb075a220ce80b058ab7bca140a";
    auto expectedEncoded =
        (boost::format(R"({"nonce":89,"value":"0","receiver":"%1%","sender":"%2%","senderUsername":"%3%","receiverUsername":"%4%","gasPrice":1000000000,"gasLimit":50000,"chainID":"local-testnet","version":1,"signature":"%5%"})") % BOB_BECH32 % ALICE_BECH32
         // "alice"
         % "YWxpY2U="
         // "bob"
         % "Ym9i" % expectedSignature)
            .str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignWithOptions) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_generic_action()->mutable_accounts()->set_sender_nonce(89);
    input.mutable_generic_action()->mutable_accounts()->set_sender(ALICE_BECH32);
    input.mutable_generic_action()->mutable_accounts()->set_receiver(BOB_BECH32);
    input.mutable_generic_action()->set_value("0");
    input.mutable_generic_action()->set_data("");
    input.mutable_generic_action()->set_version(1);
    // We'll set a dummy value on the "options" field (merely an example).
    // Currently, the "options" field should be ignored (not set) by applications using TW Core.
    // In the future, TW Core will handle specific transaction options
    // (such as the "SignedWithHash" flag, as seen in https://github.com/ElrondNetwork/elrond-go-core/blob/main/core/versioning/txVersionChecker.go)
    // when building and signing transactions.
    input.mutable_generic_action()->set_options(42);
    input.set_gas_price(1000000000);
    input.set_gas_limit(50000);
    input.set_chain_id("local-testnet");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "d9a624f13960ae1cc471de48bdb43b101b9d469bb8b159f68bb629bb32d0109e1acfebb62d6d2fc5786c0b85f9e7ce2caff74988864a8285f34797c5a5fa5801";
    auto expectedEncoded =
        (boost::format(R"({"nonce":89,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"local-testnet","version":1,"options":42,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
    ASSERT_EQ(expectedSignature, signature);
}

TEST(ElrondSigner, SignEGLDTransfer) {
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
    auto expectedSignature = "7e1c4c63b88ea72dcf7855a54463b1a424eb357ac3feb4345221e512ce07c7a50afb6d7aec6f480b554e32cf2037082f3bc17263d1394af1f3ef240be53c930b";
    auto expectedEncoded =
        (boost::format(R"({"nonce":7,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignESDTTransfer) {
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
    auto expectedSignature = "9add6d9ac3f1a1fddb07b934e8a73cad3b8c232bdf29d723c1b38ad619905f03e864299d06eb3fe3bbb48a9f1d9b7f14e21dc5eaffe0c87f5718ad0c4198bb0c";
    auto expectedEncoded =
        (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":425000,"data":"%3%","chainID":"1","version":1,"signature":"%4%"})") % BOB_BECH32 % ALICE_BECH32
         // "ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000"
         % "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA=" % expectedSignature)
            .str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignESDTNFTTransfer) {
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
    auto expectedSignature = "cc935685d5b31525e059a16a832cba98dee751983a5a93de4198f6553a2c55f5f1e0b4300fe9077376fa754546da0b0f6697e66462101a209aafd0fc775ab60a";
    auto expectedEncoded =
        (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":937500,"data":"%3%","chainID":"1","version":1,"signature":"%4%"})") % ALICE_BECH32 % ALICE_BECH32
         // "ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8"
         % "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg=" % expectedSignature)
            .str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

} // namespace TW::Elrond::tests
