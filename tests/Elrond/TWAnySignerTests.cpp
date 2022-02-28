// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include "boost/format.hpp"

#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"
#include "Elrond/Signer.h"
#include "TestAccounts.h"

using namespace TW;
using namespace TW::Elrond;


TEST(TWAnySignerElrond, Sign) {
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

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeElrond);

    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700";
    auto expectedEncoded = (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(TWAnySignerElrond, SignJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = STRING((boost::format(R"({"genericAction" : {"accounts": {"senderNonce": 7, "receiver": "%1%", "sender": "%2%"}, "data": "foo", "value": "0", "version": 1}, "gasPrice": 1000000000, "gasLimit": 50000, "chainId": "1"})") % BOB_BECH32 % ALICE_BECH32).str().c_str());
    auto privateKey = DATA(ALICE_SEED_HEX);
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeElrond));
    auto expectedSignature = "e8647dae8b16e034d518a1a860c6a6c38d16192d0f1362833e62424f424e5da660770dff45f4b951d9cc58bfb9d14559c977d443449bfc4b8783ff9c84065700";
    auto expectedEncoded = (boost::format(R"({"nonce":7,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeElrond));
    assertStringsEqual(encoded, expectedEncoded.c_str());
}
