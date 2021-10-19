// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "Cosmos/Protobuf/bank_tx.pb.h"
#include "Cosmos/Protobuf/tx.pb.h"
#include "Data.h"
#include "HexCoding.h"
#include "Base64.h"

#include "Protobuf/Article.pb.h"
#include "../interface/TWTestUtilities.h"

#include <google/protobuf/util/json_util.h>
#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

using namespace TW::Cosmos;
using namespace TW;
using json = nlohmann::json;


TEST(CosmosProtobuf, SendMsg) {
    auto msgSend = cosmos::bank::v1beta1::MsgSend();
    msgSend.set_from_address("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02");
    msgSend.set_to_address("cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573");
    auto coin = msgSend.add_amount();
    coin->set_denom("muon");
    coin->set_amount("1");

    auto txBody = cosmos::TxBody();
    txBody.add_messages()->PackFrom(msgSend);
    txBody.set_memo("");
    txBody.set_timeout_height(0);

    const auto serialized = data(txBody.SerializeAsString());
    EXPECT_EQ(hex(serialized), "0a9c010a2f747970652e676f6f676c65617069732e636f6d2f636f736d6f732e62616e6b2e763162657461312e4d736753656e6412690a2d636f736d6f733168736b366a727979716a6668703564686335357463396a74636b796778306570683664643032122d636f736d6f73317a743530617a7570616e716c66616d356166687633686578777975746e756b656834633537331a090a046d756f6e120131");
    EXPECT_EQ(Base64::encode(serialized), "CpwBCi90eXBlLmdvb2dsZWFwaXMuY29tL2Nvc21vcy5iYW5rLnYxYmV0YTEuTXNnU2VuZBJpCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISLWNvc21vczF6dDUwYXp1cGFucWxmYW01YWZodjNoZXh3eXV0bnVrZWg0YzU3MxoJCgRtdW9uEgEx");

    std::string json;
    google::protobuf::util::MessageToJsonString(txBody, &json);
    assertJSONEqual(json, R"({"messages":[{"@type":"type.googleapis.com/cosmos.bank.v1beta1.MsgSend","amount":[{"amount":"1","denom":"muon"}],"fromAddress":"cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02","toAddress":"cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"}]})");
}

TEST(CosmosProtobuf, DeterministicSerialization_Article) {
    // https://github.com/cosmos/cosmos-sdk/blob/master/docs/architecture/adr-027-deterministic-protobuf-serialization.md
    auto article = blog::Article();
    article.set_title("The world needs change 🌳");
    article.set_description("");
    article.set_created(1596806111080);
    article.set_updated(0);
    article.set_public_(true);
    article.set_promoted(false);
    article.set_type(blog::NEWS);
    article.set_review(blog::REVIEW_UNSPECIFIED);
    *article.add_comments() = "Nice one";
    *article.add_comments() = "Thank you";

    const auto serialized = data(article.SerializeAsString());
    EXPECT_EQ(hex(serialized), "0a1b54686520776f726c64206e65656473206368616e676520f09f8cb318e8bebec8bc2e280138024a084e696365206f6e654a095468616e6b20796f75");
}
