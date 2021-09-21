// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "proto/Cosmos.pb.h"
#include "Cosmos/Signer.h"
#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

TEST(CryptoorgSigner, SignTx) {
    auto input = Cosmos::Proto::SigningInput();

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0");
    message.set_to_address("cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("basecro");
    amountOfTx->set_amount(100000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("basecro");
    amountOfFee->set_amount(2000);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"fee":{"amounts":[{"denom":"basecro","amount":"2000"}],"gas":"200000"},"messages":[{"sendCoinsMessage":{"fromAddress":"cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0","toAddress":"cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus","amounts":[{"denom":"basecro","amount":"100000000"}]}}]})", json);

    auto privateKey = parse_hex("200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d");
    {
        auto privateKeyKey = PrivateKey(privateKey);
        auto publicKey = privateKeyKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        auto address = Address("cro", publicKey);
        EXPECT_EQ("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0", address.string());
    }
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Signer::sign(input);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"2000","denom":"basecro"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"100000000","denom":"basecro"}],"from_address":"cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0","to_address":"cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A4gxsGFiPn6L5Z2IjHEISkXI0IkwfL9exV3GLB171Wvj"},"signature":"l1IbecABw0nyWqAS52/BGCqB84sPmMQow5JKGAAdMG0BNBDsGzgFyOWbeQLhi6vw1O8Q3E1oZHTGjBTobn+qKw=="}]}})", output.json());
    EXPECT_EQ(hex(output.signature()), "97521b79c001c349f25aa012e76fc1182a81f38b0f98c428c3924a18001d306d013410ec1b3805c8e59b7902e18babf0d4ef10dc4d686474c68c14e86e7faa2b");
}
