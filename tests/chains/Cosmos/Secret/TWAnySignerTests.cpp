// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

TEST(TWAnySignerSecret, Sign) {
    auto privateKey = parse_hex("87201512d132ef7a1e57f9e24905fbc24300bd73f676b5716182be5f3e39dada");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(265538);
    input.set_chain_id("secret-4");
    input.set_memo("");
    input.set_sequence(1);
    input.set_private_key(privateKey.data(), privateKey.size());

    Address fromAddress;
    Address toAddress;
    EXPECT_TRUE(Address::decode("secret18mdrja40gfuftt5yx6tgj0fn5lurplezyp894y", fromAddress));
    EXPECT_TRUE(Address::decode("secret1rnq6hjfnalxeef87rmdeya3nu9dhpc7k9pujs3", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("uscrt");
    amountOfTx->set_amount("100000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(25000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uscrt");
    amountOfFee->set_amount("2500");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSecret);

    // https://www.mintscan.io/secret/txs/01F4BD2458BF966F287533775C8D67BBC7CA7214CAEB1752D270A90223E9E82F
    assertJSONEqual(output.serialized(), "{\"tx_bytes\":\"CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLXNlY3JldDE4bWRyamE0MGdmdWZ0dDV5eDZ0Z2owZm41bHVycGxlenlwODk0eRItc2VjcmV0MXJucTZoamZuYWx4ZWVmODdybWRleWEzbnU5ZGhwYzdrOXB1anMzGg8KBXVzY3J0EgYxMDAwMDASZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAkZqxdKMtPq2w0kGDGwWGejTAed0H7azPMHtrCX0XYZGEgQKAggBGAESEwoNCgV1c2NydBIEMjUwMBCowwEaQOcHd2gHpa5WKZ/5RRerEtrHlyDlojIEzUGhC9xMFgs7UQMWy+kTTN+NRf7zQ8rx3cPkIKeZhv0u1KRc8uRCc4o=\",\"mode\":\"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "e707776807a5ae56299ff94517ab12dac79720e5a23204cd41a10bdc4c160b3b510316cbe9134cdf8d45fef343caf1ddc3e420a79986fd2ed4a45cf2e442738a");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

} // namespace TW::Cosmos::tests
