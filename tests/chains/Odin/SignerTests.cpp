// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "proto/Cosmos.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cosmos;

TEST(OdinSigner, SignTransfer_81B4) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(124703);
    input.set_chain_id("odin-mainnet-freya");
    input.set_memo("");
    input.set_sequence(0);

    Address fromAddress;
    Address toAddress;
    EXPECT_TRUE(Address::decode("odin1fzjnsuwzxznqd3qlkt5njd4vvpcjqca6zckfux", fromAddress));
    EXPECT_TRUE(Address::decode("odin13d7rfa8v8jmmsh63jetd9euqftaqfc3hre5ake", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("loki");
    amountOfTx->set_amount("99800");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("loki");
    amountOfFee->set_amount("200");

    auto privateKey = parse_hex("a39b28abca9b410e6f53d87a42229150cb176211996391c989336fbbcb9606cb");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeOsmosis);

    assertJSONEqual(output.serialized(),
                    "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":"
                    "\"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKK"
                    "29kaW4xZnpqbnN1d3p4em5xZDNxbGt0NW5qZDR2dnBjanFjYTZ6Y2tmdXgSK29kaW4xM2Q3cmZhOHY"
                    "4am1tc2g2M2pldGQ5ZXVxZnRh"
                    "cWZjM2hyZTVha2UaDQoEbG9raRIFOTk4MDASYwpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS"
                    "5QdWJLZXkSIwohAzdXdUgXyzt"
                    "VVpl5pFOdmj8kUEWei9xLNcZQihXXmt5IEgQKAggBEhEKCwoEbG9raRIDMjAwEMCaDBpAfXlsIu1Ke"
                    "+kalRDx2vkE8anvcsajlru8RE"
                    "TlqBqEQWYD2JENyS0fmxTNsEmNu40Unjth3c0O8aUFYnj4D38P3A==\"}");
    EXPECT_EQ(hex(output.signature()),
              "7d796c22ed4a7be91a9510f1daf904f1a9ef72c6a396bbbc4444e5a81a84416603d8910dc92d1f9b14cd"
              "b0498dbb8d149e3b61ddcd0ef1a5056278f80f7f0fdc");
    EXPECT_EQ(output.error(), "");
    EXPECT_EQ(output.json(), "");
}
