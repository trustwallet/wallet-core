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

    auto& fee = *input.mutable_fee();
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

    EXPECT_EQ(output.serialized(), "CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H");
    EXPECT_EQ(hex(output.signature()), "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(CosmosSigner, SignProtobuf_ErrorMissingMessage) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    EXPECT_EQ(output.error(), "Error: No message found");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(hex(output.signature()), "");
}

TEST(CosmosSigner, SignTxJson) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON); // obsolete
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

    auto& fee = *input.mutable_fee();
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
    EXPECT_EQ(output.error(), "");
}

TEST(CosmosSigner, SignTxJson_WithMode) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON); // obsolete
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

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount(200);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    {
        auto output = Signer::sign(input);
        EXPECT_EQ(R"({"mode":"async","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
        EXPECT_EQ(output.error(), "");
    }
    input.set_mode(Proto::BroadcastMode::SYNC);
    {
        auto output = Signer::sign(input);
        EXPECT_EQ(R"({"mode":"sync","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
        EXPECT_EQ(output.error(), "");
    }
}

TEST(CosmosSigner, SignIbcTransferJson) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON);
    input.set_account_number(1037);
    input.set_chain_id("cosmoshub-4");
    input.set_memo("");
    input.set_sequence(1);

    Address fromAddress;
    EXPECT_TRUE(Address::decode("cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx", fromAddress));
    Address toAddress;
    EXPECT_TRUE(Address::decode("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_transfer_tokens_message();
    message.set_source_port("transfer");
    message.set_source_channel("channel-141");
    message.set_sender(fromAddress.string());
    message.set_receiver(toAddress.string());
    message.mutable_token()->set_denom("uatom");
    message.mutable_token()->set_amount(100000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount(12500);

    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    EXPECT_EQ(output.json(), R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"12500","denom":"uatom"}],"gas":"500000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgTransfer","value":{"receiver":"osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5","sender":"cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx","source_channel":"channel-141","source_port":"transfer","token":{"amount":"100000","denom":"uatom"}}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJ"},"signature":"Vlcg0DtgxcbnnBse1gHnDFofpzOdagdMeCa4jpOcJKNMZNcYSz5klYEgGeSmUQM+tmDXQLUoDdIP+xpOc1SPFw=="}]}})");
    EXPECT_EQ(hex(output.signature()), "565720d03b60c5c6e79c1b1ed601e70c5a1fa7339d6a074c7826b88e939c24a34c64d7184b3e6495812019e4a651033eb660d740b5280dd20ffb1a4e73548f17");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(CosmosSigner, SignIbcTransferProtobuf) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("cosmoshub-4");
    input.set_memo("");
    input.set_sequence(1);

    Address fromAddress;
    EXPECT_TRUE(Address::decode("cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx", fromAddress));
    Address toAddress;
    EXPECT_TRUE(Address::decode("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_transfer_tokens_message();
    message.set_source_port("transfer");
    message.set_source_channel("channel-141");
    message.set_sender(fromAddress.string());
    message.set_receiver(toAddress.string());
    message.mutable_token()->set_denom("uatom");
    message.mutable_token()->set_amount(100000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount(12500);

    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    EXPECT_EQ(output.serialized(), "CrcBCrQBCikvaWJjLmFwcGxpY2F0aW9ucy50cmFuc2Zlci52MS5Nc2dUcmFuc2ZlchKGAQoIdHJhbnNmZXISC2NoYW5uZWwtMTQxGg8KBXVhdG9tEgYxMDAwMDAiLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseCorb3NtbzFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MGVuOTdmNTIAEmgKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQLs71zkN6MCxn+VRo3ksx826RH0Z9fmpStBweE+HVY2SRIECgIIARgBEhQKDgoFdWF0b20SBTEyNTAwEKDCHhpAi/bh0G4zABR62EUCt3C2yj9WvC1afsEvEhAdk9JcZm03MuoK0MCT09qbyIQVgoRASOgWjodUiCBa0P39zCcXMQ==");
    EXPECT_EQ(hex(output.signature()), "8bf6e1d06e3300147ad84502b770b6ca3f56bc2d5a7ec12f12101d93d25c666d3732ea0ad0c093d3da9bc8841582844048e8168e875488205ad0fdfdcc271731");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error(), "");
}
