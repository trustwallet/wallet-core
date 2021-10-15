// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;


TEST(CosmosSigner, SignTxProtobuf) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount(1);

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"signingMode":"Protobuf","accountNumber":"1037","chainId":"gaia-13003","fee":{"amounts":[{"denom":"muon","amount":"200"}],"gas":"200000"},"sequence":"8","messages":[{"sendCoinsMessage":{"fromAddress":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","toAddress":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573","amounts":[{"denom":"muon","amount":"1"}]}}]})", json);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    EXPECT_EQ(hex(output.serialized()), "0aa5010aa2010a35747970652e676f6f676c65617069732e636f6d2f636f736d6f732e70726f746f2e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e1201311a406f73eb152b177ffc3f264f49ea3f7c61c8de377ac035210531059b4917ea5f511f2d46cc1df11f6c13e7031a2ed380001d43b376212c2431d7a6c456cfa9099f");
    EXPECT_EQ(output.serialized_base64(), "CqUBCqIBCjV0eXBlLmdvb2dsZWFwaXMuY29tL2Nvc21vcy5wcm90by5iYW5rLnYxYmV0YTEuTXNnU2VuZBJpCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISLWNvc21vczF6dDUwYXp1cGFucWxmYW01YWZodjNoZXh3eXV0bnVrZWg0YzU3MxoJCgRtdW9uEgExGkBvc+sVKxd//D8mT0nqP3xhyN43esA1IQUxBZtJF+pfUR8tRswd8R9sE+cDGi7TgAAdQ7N2ISwkMdemxFbPqQmf");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(hex(output.signature()), "6f73eb152b177ffc3f264f49ea3f7c61c8de377ac035210531059b4917ea5f511f2d46cc1df11f6c13e7031a2ed380001d43b376212c2431d7a6c456cfa9099f");
}

TEST(CosmosSigner, SignTxJson) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount(1);

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"accountNumber":"1037","chainId":"gaia-13003","fee":{"amounts":[{"denom":"muon","amount":"200"}],"gas":"200000"},"sequence":"8","messages":[{"sendCoinsMessage":{"fromAddress":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","toAddress":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573","amounts":[{"denom":"muon","amount":"1"}]}}]})", json);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    // the sample tx on testnet
    // https://hubble.figment.network/chains/gaia-13003/blocks/142933/transactions/3A9206598C3D2E75A5EC074FD33EA53EB18EC729357F0965971C1C51F812AEA3?format=json
    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());

    EXPECT_EQ(hex(output.signature()), "fc3ef899d206c88077fec42f21ba0b4df4bd3fd115fdf606ae01d9136fef363f57e9e33a7b9ec6ddab658cd07e3c0067470de94e4e75b979a1085a29f0efd926");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.serialized_base64(), "");
}

TEST(CosmosSigner, SignTxWithMode) {
    auto input = Proto::SigningInput();
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);
    input.set_mode(Proto::BroadcastMode::ASYNC);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("muon");
    amountOfTx->set_amount(1);

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    {
        auto output = Signer::sign(input);
        ASSERT_EQ(R"({"mode":"async","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
    }
    input.set_mode(Proto::BroadcastMode::SYNC);
    {
        auto output = Signer::sign(input);
        ASSERT_EQ(R"({"mode":"sync","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
    }
}
