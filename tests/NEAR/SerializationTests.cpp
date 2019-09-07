// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "Base58.h"
#include "proto/NEAR.pb.h"
#include "NEAR/Serialization.h"
#include <iostream>

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(NEARSerialization, SerializeTransferTransaction) {
    auto publicKey = Base58::bitcoin.decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.mutable_public_key()->set_key_type(0);
    input.mutable_public_key()->set_data(publicKey.data(), publicKey.size());
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto &transfer = *input.mutable_actions(0)->mutable_transfer();
    Data deposit(16, 0);
    deposit[0] = 1;
    transfer.set_deposit(deposit.data(), deposit.size());

    auto blockHash = Base58::bitcoin.decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto serialized = transactionData(input);
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000301000000000000000000000000000000");
}

}
