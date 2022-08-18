// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Cosmos/Address.h"
#include "Cosmos/Protobuf/authz_tx.pb.h"
#include "Cosmos/Protobuf/bank_tx.pb.h"
#include "Cosmos/Protobuf/staking_authz.pb.h"
#include "Cosmos/Protobuf/tx.pb.h"
#include "Data.h"
#include "HexCoding.h"

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

TEST(CosmosProtobuf, AuthGrant) {
    auto stakeAuthz = cosmos::staking::v1beta1::StakeAuthorization();
    stakeAuthz.set_authorization_type(cosmos::staking::v1beta1::AUTHORIZATION_TYPE_DELEGATE);
    stakeAuthz.mutable_allow_list()->add_address("cosmosvaloper1gjtvly9lel6zskvwtvlg5vhwpu9c9waw7sxzwx");
    auto msgGrant = cosmos::authz::v1beta1::MsgGrant();
    msgGrant.set_granter("cosmos1tf0aw7dq4w3vppfqdglefs6wzyz5um2sf3x6jc");
    msgGrant.set_grantee("cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf");
    msgGrant.mutable_grant()->mutable_expiration()->set_seconds(1691445600);
    msgGrant.mutable_grant()->mutable_authorization()->set_type_url("/cosmos.staking.v1beta1.StakeAuthorization");
    //const auto ProtobufAnyNamespacePrefix = ""; // to override default 'type.googleapis.com'
    msgGrant.mutable_grant()->mutable_authorization()->PackFrom(stakeAuthz);
    auto txBody = cosmos::TxBody();
    txBody.add_messages()->PackFrom(msgGrant);
    txBody.set_memo("");
    txBody.set_timeout_height(0);
    std::string json;
    google::protobuf::util::MessageToJsonString(txBody, &json);
    std::cout << json << std::endl;
    // TODO: https://github.com/cosmos/cosmos-sdk/blob/6f070623741fe0d6851d79ada41e6e2b1c67e236/proto/cosmos/staking/v1beta1/authz.proto for staking auth
    // Verify that the serialized tx is equal to https://www.mintscan.io/cosmos/txs/2C0B5DE0E48FD166FC036F8F662C778F3A95AD270E3CEE24AE39CFE5B2A05A6F
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
