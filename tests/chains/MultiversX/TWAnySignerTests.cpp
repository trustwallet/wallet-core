// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <gtest/gtest.h>

#include "HexCoding.h"
#include "MultiversX/Signer.h"
#include "TestAccounts.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

using namespace TW;

namespace TW::MultiversX::tests {

TEST(TWAnySignerMultiversX, Sign) {
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

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeMultiversX);

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

    ASSERT_EQ(expectedSignature, signature);
    assertJSONEqual(expected, nlohmann::json::parse(encoded));
}

TEST(TWAnySignerMultiversX, SignJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = STRING(R"(
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
        })");

    auto privateKey = DATA(ALICE_SEED_HEX);
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeMultiversX));
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

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeMultiversX));
    assertJSONEqual(expected, nlohmann::json::parse(TWStringUTF8Bytes(encoded.get())));
}

} // namespace TW::MultiversX::tests
