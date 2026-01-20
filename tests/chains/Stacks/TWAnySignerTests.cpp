// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnySigner.h>

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Stacks.pb.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;

// TODO: Finalize tests

TEST(TWStacks, Sign) {
    Stacks::Proto::SigningInput input;
    auto& tf = *input.mutable_transfer();
    tf.set_to("SP2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKNRV9EJ7");
    tf.set_amount(100000);
    tf.set_fee(10000);
    tf.set_memo("hello");
    auto privateKey = PrivateKey(parse_hex("a1b2c3d4e5f60000000000000000000000000000000000000000000000000001"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    Stacks::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeStacks);
    //ASSERT_EQ(hex(output.raw_txn()), "07968dab936c1bad187c60ce4082f307d03}
}
