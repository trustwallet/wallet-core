// Copyright © 2017-2023 Trust Wallet.
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
#include "TestUtilities.h"
#include "Cosmos/Protobuf/bank_tx.pb.h"
#include "Cosmos/Protobuf/coin.pb.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos::tests {

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
    amountOfTx->set_amount("1");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"signingMode":"Protobuf","accountNumber":"1037","chainId":"gaia-13003","fee":{"amounts":[{"denom":"muon","amount":"200"}],"gas":"200000"},"sequence":"8","messages":[{"sendCoinsMessage":{"fromAddress":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","toAddress":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573","amounts":[{"denom":"muon","amount":"1"}]}}]})", json);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\": \"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H\", \"mode\": \"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47");
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
    amountOfFee->set_amount("200");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    EXPECT_EQ(output.error_message(), "Error: No message found");
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
    amountOfTx->set_amount("1");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(R"({"accountNumber":"1037","chainId":"gaia-13003","fee":{"amounts":[{"denom":"muon","amount":"200"}],"gas":"200000"},"sequence":"8","messages":[{"sendCoinsMessage":{"fromAddress":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","toAddress":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573","amounts":[{"denom":"muon","amount":"1"}]}}]})", json);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    // the sample tx on testnet
    // https://hubble.figment.network/chains/gaia-13003/blocks/142933/transactions/3A9206598C3D2E75A5EC074FD33EA53EB18EC729357F0965971C1C51F812AEA3?format=json
    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());

    EXPECT_EQ(hex(output.signature()), "fc3ef899d206c88077fec42f21ba0b4df4bd3fd115fdf606ae01d9136fef363f57e9e33a7b9ec6ddab658cd07e3c0067470de94e4e75b979a1085a29f0efd926");
}

TEST(CosmosSigner, SignTxJsonWithExecuteContractMsg) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON); // obsolete
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_execute_contract_message();
    message.set_sender(fromAddress.string());
    message.set_contract(toAddress.string());
    message.set_execute_msg("transfer");
    auto* coin = message.add_coins();
    coin->set_denom("muon");
    coin->set_amount("1");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ("{\"accountNumber\":\"1037\",\"chainId\":\"gaia-13003\",\"fee\":{\"amounts\":[{\"denom\":\"muon\",\"amount\":\"200\"}],\"gas\":\"200000\"},\"sequence\":\"8\",\"messages\":[{\"executeContractMessage\":{\"sender\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\",\"contract\":\"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573\",\"executeMsg\":\"transfer\",\"coins\":[{\"denom\":\"muon\",\"amount\":\"1\"}]}}]}", json);

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    EXPECT_EQ("{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"200\",\"denom\":\"muon\"}],\"gas\":\"200000\"},\"memo\":\"\",\"msg\":[{\"type\":\"wasm/MsgExecuteContract\",\"value\":{\"coins\":[{\"amount\":\"1\",\"denom\":\"muon\"}],\"contract\":\"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573\",\"execute_msg\":\"transfer\",\"sender\":\"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"9iQTB1Jjw8FuYPwgzVLbs1cABGYFlk3JRKGQyojQcwY/ni+9D/ViNQMb+4UuokYi74GnpPZpH5RqbJ2ju6VL2g==\"}]}}", output.json());

    EXPECT_EQ(hex(output.signature()), "f62413075263c3c16e60fc20cd52dbb35700046605964dc944a190ca88d073063f9e2fbd0ff56235031bfb852ea24622ef81a7a4f6691f946a6c9da3bba54bda");
}

TEST(CosmosSigner, SignTxJsonWithRawJSONMsg) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON); // obsolete
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");
    input.set_memo("");
    input.set_sequence(8);

    auto fromAddress = Address("cosmos", parse_hex("BC2DA90C84049370D1B7C528BC164BC588833F21"));
    auto toAddress = Address("cosmos", parse_hex("12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9"));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_raw_json_message();
    message.set_type("test");
    message.set_value("{\"test\":\"hello\"}");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    EXPECT_EQ(json, "{\"accountNumber\":\"1037\",\"chainId\":\"gaia-13003\",\"fee\":{\"amounts\":[{\"denom\":\"muon\",\"amount\":\"200\"}],\"gas\":\"200000\"},\"sequence\":\"8\",\"messages\":[{\"rawJsonMessage\":{\"type\":\"test\",\"value\":\"{\\\"test\\\":\\\"hello\\\"}\"}}]}");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    EXPECT_EQ(output.json(), "{\"mode\":\"block\",\"tx\":{\"fee\":{\"amount\":[{\"amount\":\"200\",\"denom\":\"muon\"}],\"gas\":\"200000\"},\"memo\":\"\",\"msg\":[{\"type\":\"test\",\"value\":{\"test\":\"hello\"}}],\"signatures\":[{\"pub_key\":{\"type\":\"tendermint/PubKeySecp256k1\",\"value\":\"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F\"},\"signature\":\"qhxxCOMiVhP7e7Mx+98HUZI0t5DNOFXwzIqNQz+fT6hDKR/ebW0uocsYnE5CiBNEalmBcs5gSIJegNkHhgyEmA==\"}]}}");

    EXPECT_EQ(hex(output.signature()), "aa1c7108e3225613fb7bb331fbdf07519234b790cd3855f0cc8a8d433f9f4fa843291fde6d6d2ea1cb189c4e428813446a598172ce6048825e80d907860c8498");
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
    amountOfTx->set_amount("1");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("muon");
    amountOfFee->set_amount("200");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    {
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        EXPECT_EQ(R"({"mode":"async","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
        EXPECT_EQ(output.error_message(), "");
    }
    input.set_mode(Proto::BroadcastMode::SYNC);
    {
        auto output = Signer::sign(input, TWCoinTypeCosmos);
        EXPECT_EQ(R"({"mode":"sync","tx":{"fee":{"amount":[{"amount":"200","denom":"muon"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"1","denom":"muon"}],"from_address":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","to_address":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"},"signature":"/D74mdIGyIB3/sQvIboLTfS9P9EV/fYGrgHZE2/vNj9X6eM6e57G3atljNB+PABnRw3pTk51uXmhCFop8O/ZJg=="}]}})", output.json());
        EXPECT_EQ(output.error_message(), "");
    }
}

TEST(CosmosSigner, SignIbcTransferProtobuf_817101) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(546179);
    input.set_chain_id("cosmoshub-4");
    input.set_sequence(2);

    Address fromAddress;
    EXPECT_TRUE(Address::decode("cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx", fromAddress));
    Address toAddress;
    EXPECT_TRUE(Address::decode("osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_transfer_tokens_message();
    message.set_source_port("transfer");
    message.set_source_channel("channel-141");
    message.set_sender(fromAddress.string());
    message.set_receiver(toAddress.string());
    message.mutable_token()->set_denom("uatom");
    message.mutable_token()->set_amount("100000"); // 0.1 ATOM
    message.mutable_timeout_height()->set_revision_number(1);
    message.mutable_timeout_height()->set_revision_height(8800000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount("12500");

    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    EXPECT_EQ(Cosmos::Address(TWCoinTypeCosmos, PrivateKey(privateKey).getPublicKey(TWPublicKeyTypeSECP256k1)).string(), "cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    // real-world tx: https://www.mintscan.io/cosmos/txs/817101F3D96314AD028733248B28BAFAD535024D7D2C8875D3FE31DC159F096B
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "Cr4BCr...1yKOU=", "mode": "BROADCAST_MODE_BLOCK"}' https://api.cosmos.network/cosmos/tx/v1beta1/txs
    // also similar TX: BCDAC36B605576C8182C2829C808B30A69CAD4959D5ED1E6FF9984ABF280D603
    assertJSONEqual(output.serialized(), "{\"tx_bytes\": \"Cr4BCrsBCikvaWJjLmFwcGxpY2F0aW9ucy50cmFuc2Zlci52MS5Nc2dUcmFuc2ZlchKNAQoIdHJhbnNmZXISC2NoYW5uZWwtMTQxGg8KBXVhdG9tEgYxMDAwMDAiLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseCorb3NtbzE4czBoZG5zbGxnY2Nsd2V1OWF5bXc0bmdrdHIyazBya3ZuN2ptbjIHCAEQgI6ZBBJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEC7O9c5DejAsZ/lUaN5LMfNukR9GfX5qUrQcHhPh1WNkkSBAoCCAEYAhIUCg4KBXVhdG9tEgUxMjUwMBCgwh4aQK0HIWdFMk+C6Gi1KG/vELe1ffcc1aEWUIqz2t/ZhwqNNHxUUSp27wteiugHEMVTEIOBhs84t2gIcT/nD/1yKOU=\", \"mode\": \"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "ad07216745324f82e868b5286fef10b7b57df71cd5a116508ab3dadfd9870a8d347c54512a76ef0b5e8ae80710c55310838186cf38b76808713fe70ffd7228e5");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(CosmosSigner, SignDirect1) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1037);
    input.set_chain_id("gaia-13003");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_sign_direct_message();
    const auto bodyBytes = parse_hex("0a89010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e120131");
    message.set_body_bytes(bodyBytes.data(), bodyBytes.size());
    const auto authInfoBytes = parse_hex("0a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a210257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc512040a020801180812110a0b0a046d756f6e120332303010c09a0c");
    message.set_auth_info_bytes(authInfoBytes.data(), authInfoBytes.size());

    {
        std::string json;
        google::protobuf::util::MessageToJsonString(input, &json);
        EXPECT_EQ(R"({"signingMode":"Protobuf","accountNumber":"1037","chainId":"gaia-13003","messages":[{"signDirectMessage":{"bodyBytes":"CokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATE=","authInfoBytes":"ClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECVyhuw/N9M1V7u6oACyd0SskCOqmWfK51oYHR/5H6ncUSBAoCCAEYCBIRCgsKBG11b24SAzIwMBDAmgw="}}]})", json);
    }

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\": \"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H\", \"mode\": \"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "f9e1f4001657a42009c4eb6859625d2e41e961fc72efd2842909c898e439fc1f549916e4ecac676ee353c7d54c5ae30a29b4210b8bff0ebfdcb375e105002f47");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(CosmosSigner, SignDirect_0a90010a) {
    const auto bodyBytes = parse_hex("0a90010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412700a2d636f736d6f7331706b707472653766646b6c366766727a6c65736a6a766878686c63337234676d6d6b38727336122d636f736d6f7331717970717870713971637273737a673270767871367273307a716733797963356c7a763778751a100a0575636f736d120731323334353637");
    { // verify contents of body
        auto msgSend = cosmos::bank::v1beta1::MsgSend();
        msgSend.set_from_address("cosmos1pkptre7fdkl6gfrzlesjjvhxhlc3r4gmmk8rs6");
        msgSend.set_to_address("cosmos1qypqxpq9qcrsszg2pvxq6rs0zqg3yyc5lzv7xu");
        auto amount = msgSend.add_amount();
        amount->set_denom("ucosm");
        amount->set_amount("1234567");
        const auto msgSendSer = msgSend.SerializeAsString();
        const auto bodyBytes1 = data(msgSendSer);
        ASSERT_EQ(hex(bodyBytes1), "0a2d636f736d6f7331706b707472653766646b6c366766727a6c65736a6a766878686c63337234676d6d6b38727336122d636f736d6f7331717970717870713971637273737a673270767871367273307a716733797963356c7a763778751a100a0575636f736d120731323334353637");
        const auto prefix = "/cosmos.bank.v1beta1.MsgSend";
        const auto bodyBytes2 = parse_hex("0a90010a1c" + hex(data(prefix)) + "1270" + hex(bodyBytes1));
        ASSERT_EQ(hex(bodyBytes2), hex(bodyBytes));
    }

    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1);
    input.set_chain_id("cosmoshub-4");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_sign_direct_message();
    message.set_body_bytes(bodyBytes.data(), bodyBytes.size());
    const auto authInfoBytes = parse_hex("0a0a0a0012040a020801180112130a0d0a0575636f736d12043230303010c09a0c");
    message.set_auth_info_bytes(authInfoBytes.data(), authInfoBytes.size());

    {
        std::string json;
        google::protobuf::util::MessageToJsonString(input, &json);
        EXPECT_EQ(R"({"signingMode":"Protobuf","accountNumber":"1","chainId":"cosmoshub-4","messages":[{"signDirectMessage":{"bodyBytes":"CpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKLWNvc21vczFwa3B0cmU3ZmRrbDZnZnJ6bGVzamp2aHhobGMzcjRnbW1rOHJzNhItY29zbW9zMXF5cHF4cHE5cWNyc3N6ZzJwdnhxNnJzMHpxZzN5eWM1bHp2N3h1GhAKBXVjb3NtEgcxMjM0NTY3","authInfoBytes":"CgoKABIECgIIARgBEhMKDQoFdWNvc20SBDIwMDAQwJoM"}}]})", json);
    }

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);

    assertJSONEqual(output.serialized(), "{\"tx_bytes\": \"CpMBCpABChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnAKLWNvc21vczFwa3B0cmU3ZmRrbDZnZnJ6bGVzamp2aHhobGMzcjRnbW1rOHJzNhItY29zbW9zMXF5cHF4cHE5cWNyc3N6ZzJwdnhxNnJzMHpxZzN5eWM1bHp2N3h1GhAKBXVjb3NtEgcxMjM0NTY3EiEKCgoAEgQKAggBGAESEwoNCgV1Y29zbRIEMjAwMBDAmgwaQEgXmSAlm4M5bz+OX1GtvvZ3fBV2wrZrp4A/Imd55KM7ASivB/siYJegmYiOKzQ82uwoEmFalNnG2BrHHDwDR2Y=\", \"mode\": \"BROADCAST_MODE_BLOCK\"}");
    EXPECT_EQ(hex(output.signature()), "48179920259b83396f3f8e5f51adbef6777c1576c2b66ba7803f226779e4a33b0128af07fb226097a099888e2b343cdaec2812615a94d9c6d81ac71c3c034766");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(CosmosSigner, MsgVote) {
    // Successfully broadcasted https://www.mintscan.io/cosmos/txs/2EFA054B842B1641B131137B13360F95164C6C1D51BB4A4AC6DE8F75F504AA4C
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(1366160);
    input.set_chain_id("cosmoshub-4");
    input.set_memo("");
    input.set_sequence(0);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_msg_vote();
    message.set_voter("cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4");
    message.set_proposal_id(77);
    message.set_option(TW::Cosmos::Proto::Message_VoteOption_YES);

    auto& fee = *input.mutable_fee();
    fee.set_gas(97681);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount("2418");

    auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeCosmos);
    auto expected = R"(
                {"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"ClQKUgobL2Nvc21vcy5nb3YudjFiZXRhMS5Nc2dWb3RlEjMITRItY29zbW9zMW1yeTQ3cGtnYTV0ZHN3dGx1eTBtOHRlc2xwYWxrZHEwN3Bzd3U0GAESZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBEhMKDQoFdWF0b20SBDI0MTgQkfsFGkA+Nb3NULc38quGC1x+8ZXry4w9mMX3IA7wUjFboTv7kVOwPlleIc8UqIsjVvKTUFnUuW8dlGQzNR1KkvbvZ1NA"})";
    assertJSONEqual(output.serialized(), expected);
}

} // namespace TW::Cosmos::tests
