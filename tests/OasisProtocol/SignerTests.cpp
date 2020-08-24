// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OasisProtocol/Signer.h"
#include "OasisProtocol/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::OasisProtocol;

// TODO: Add tests

TEST(OasisProtocolSigner, Sign) {
    auto input = Proto::SigningInput();
    auto &transfer = *input.mutable_transfer();
    transfer.set_gas_price(0);
    transfer.set_gas_amount("0");
    transfer.set_nonce(0);
    transfer.set_to("oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5");
    transfer.set_method("staking.Transfer");
    transfer.set_amount("10000000");
    transfer.set_context("oasis-core/consensus: tx for chain bc1c715319132305795fa86bd32e93291aaacbfb5b5955f3ba78bdba413af9e1");

    auto key = parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()),"82870000420030404900313030303030303000707374616b696e672e5472616e7366657258404b49c787bcff88cdaa39f357a78f094f7573ddc4b004eeb8ac333a43c66fba0e9b52310041c969f37d14f9d5f034c587863a09660188e7858cc330110671ad0e");
}
