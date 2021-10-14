// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Protobuf/Article.pb.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(CosmosProtobufSerialization, DeterministicSerialization_Article) {
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
