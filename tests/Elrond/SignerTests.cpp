// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include "boost/format.hpp"

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Elrond/Signer.h"
#include "Elrond/Address.h"
#include "TestAccounts.h"

using namespace TW;
using namespace TW::Elrond;


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
    (
        boost::format(R"({"nonce":89,"value":"0","receiver":"%1%","sender":"%2%","senderUsername":"%3%","receiverUsername":"%4%","gasPrice":1000000000,"gasLimit":50000,"chainID":"local-testnet","version":1,"signature":"%5%"})")
        % BOB_BECH32 
        % ALICE_BECH32 
        // "alice"
        % "YWxpY2U=" 
        // "bob"
        % "Ym9i" 
        % expectedSignature
    ).str();

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
    (
        boost::format(R"({"nonce":89,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"local-testnet","version":1,"options":42,"signature":"%3%"})")
        % BOB_BECH32 
        % ALICE_BECH32 
        % expectedSignature
    ).str();

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
    (
        boost::format(R"({"nonce":7,"value":"1000000000000000000","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1,"signature":"%3%"})")
        % BOB_BECH32 
        % ALICE_BECH32 
        % expectedSignature
    ).str();

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
    (
        boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":425000,"data":"%3%","chainID":"1","version":1,"signature":"%4%"})")
        % BOB_BECH32 
        % ALICE_BECH32
        // "ESDTTransfer@4d59544f4b454e2d31323334@09184e72a000"
        % "RVNEVFRyYW5zZmVyQDRkNTk1NDRmNGI0NTRlMmQzMTMyMzMzNEAwOTE4NGU3MmEwMDA="
        % expectedSignature
    ).str();

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
    (
        boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":937500,"data":"%3%","chainID":"1","version":1,"signature":"%4%"})")
        % ALICE_BECH32 
        % ALICE_BECH32
        // "ESDTNFTTransfer@4c4b4d45582d616162393130@04@028ec3dfa01ac000@8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8"
        % "RVNEVE5GVFRyYW5zZmVyQDRjNGI0ZDQ1NTgyZDYxNjE2MjM5MzEzMEAwNEAwMjhlYzNkZmEwMWFjMDAwQDgwNDlkNjM5ZTVhNjk4MGQxY2QyMzkyYWJjY2U0MTAyOWNkYTc0YTE1NjM1MjNhMjAyZjA5NjQxY2MyNjE4Zjg="
        % expectedSignature
    ).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}
