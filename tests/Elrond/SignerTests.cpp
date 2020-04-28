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


TEST(ElrondSigner, Sign) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_transaction()->set_nonce(0);
    input.mutable_transaction()->set_value("0");
    input.mutable_transaction()->set_sender(ALICE_BECH32);
    input.mutable_transaction()->set_receiver(BOB_BECH32);
    input.mutable_transaction()->set_gas_price(200000000000000);
    input.mutable_transaction()->set_gas_limit(500000000);
    input.mutable_transaction()->set_data("foo");

    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "b88ad2fe98a7316ea432a0a76c18cd87200fe75f27a8f053ea6532b40317dbec5136c5463aef132ae951b7e60d45d921caaa5903e70821dcda98f237d4ec4308";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000,"data":"foo","signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = (boost::format(R"({"transaction" : {"data":"foo","value":"0","nonce":0,"receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000}})") % BOB_BECH32 % ALICE_BECH32).str();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    
    auto encoded = Signer::signJSON(input, privateKey.bytes);
    auto expectedSignature = "b88ad2fe98a7316ea432a0a76c18cd87200fe75f27a8f053ea6532b40317dbec5136c5463aef132ae951b7e60d45d921caaa5903e70821dcda98f237d4ec4308";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000,"data":"foo","signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignWithoutData) {
    auto input = Proto::SigningInput();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    input.mutable_transaction()->set_nonce(0);
    input.mutable_transaction()->set_value("0");
    input.mutable_transaction()->set_sender(ALICE_BECH32);
    input.mutable_transaction()->set_receiver(BOB_BECH32);
    input.mutable_transaction()->set_gas_price(200000000000000);
    input.mutable_transaction()->set_gas_limit(500000000);
    input.mutable_transaction()->set_data("");
    
    auto output = Signer::sign(input);
    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "39ab0e18bfce04bf53c9610faa3b9e7cecfca919510a7631e529e9086279b70a4832df32a5d1b8fdceb4e9082f2995da97f9195532c8d611ee749bc312cbf90c";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000,"data":"","signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(ElrondSigner, SignJSONWithoutData) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = (boost::format(R"({"transaction" : {"value":"0","nonce":0,"receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000}})") % BOB_BECH32 % ALICE_BECH32).str();
    auto privateKey = PrivateKey(parse_hex(ALICE_SEED_HEX));
    
    auto encoded = Signer::signJSON(input, privateKey.bytes);
    auto expectedSignature = "39ab0e18bfce04bf53c9610faa3b9e7cecfca919510a7631e529e9086279b70a4832df32a5d1b8fdceb4e9082f2995da97f9195532c8d611ee749bc312cbf90c";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":200000000000000,"gasLimit":500000000,"data":"","signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedEncoded, encoded);
}
