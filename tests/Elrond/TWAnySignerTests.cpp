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

    input.mutable_transaction()->set_nonce(0);
    input.mutable_transaction()->set_value("0");
    input.mutable_transaction()->set_sender(ALICE_BECH32);
    input.mutable_transaction()->set_receiver(BOB_BECH32);
    input.mutable_transaction()->set_gas_price(1000000000);
    input.mutable_transaction()->set_gas_limit(50000);
    input.mutable_transaction()->set_data("foo");
    input.mutable_transaction()->set_chain_id("1");
    input.mutable_transaction()->set_version(1);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeElrond);

    auto signature = output.signature();
    auto encoded = output.encoded();
    auto expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_EQ(expectedSignature, signature);
    ASSERT_EQ(expectedEncoded, encoded);
}

TEST(TWAnySignerElrond, SignJSON) {
    // Shuffle some fields, assume arbitrary order in the input
    auto input = STRING((boost::format(R"({"transaction" : {"data":"foo","value":"0","nonce":0,"receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"chainId":"1","version":1}})") % BOB_BECH32 % ALICE_BECH32).str().c_str());
    auto privateKey = DATA(ALICE_SEED_HEX);
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeElrond));
    auto expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";
    auto expectedEncoded = (boost::format(R"({"nonce":0,"value":"0","receiver":"%1%","sender":"%2%","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1,"signature":"%3%"})") % BOB_BECH32 % ALICE_BECH32 % expectedSignature).str();

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeElrond));
    assertStringsEqual(encoded, expectedEncoded.c_str());
}
