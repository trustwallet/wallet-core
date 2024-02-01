// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnySigner.h>
#include "Hedera/Signer.h"
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Hedera::tests {

TEST(TWAnySignerHedera, Sign) {
    // Successfully broadcasted: https://hashscan.io/testnet/transaction/0.0.48694347-1667222879-749068449?t=1667222891.440398729&p=1
    Proto::SigningInput input;
    auto privateKey = PrivateKey(parse_hex("e87a5584c0173263e138db689fdb2a7389025aaae7cb1a18a1017d76012130e8"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto* body = input.mutable_body();

    *body->mutable_memo() = "";
    *body->mutable_nodeaccountid() = "0.0.9";
    body->set_transactionfee(100000000);
    body->set_transactionvalidduration(120);
    auto* transferMsg = body->mutable_transfer();
    transferMsg->set_from("0.0.48694347");
    transferMsg->set_to("0.0.48462050");
    transferMsg->set_amount(100000000);

    auto* transactionID = body->mutable_transactionid();
    transactionID->mutable_transactionvalidstart()->set_seconds(1667222879);
    transactionID->mutable_transactionvalidstart()->set_nanos(749068449);
    transactionID->set_accountid("0.0.48694347");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeHedera);
    ASSERT_EQ(hex(output.encoded()), "0a440a150a0c08df9aff9a0610a1c197e502120518cb889c17120218091880c2d72f22020878721e0a1c0a0c0a0518e2f18d17108084af5f0a0c0a0518cb889c1710ff83af5f12660a640a205d3a70d08b2beafb72c7a68986b3ff819a306078b8c359d739e4966e82e6d40e1a40612589c3b15f1e3ed6084b5a3a5b1b81751578cac8d6c922f31731b3982a5bac80a22558b2197276f5bae49b62503a4d39448ceddbc5ef3ba9bee4c0f302f70c");
}

}
