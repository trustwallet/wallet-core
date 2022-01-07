// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Common.pb.h"
#include "proto/Ripple.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ripple;

TEST(TWAnySignerRipple, Sign) {
    auto key = parse_hex("ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764");
    Proto::SigningInput input;

    input.set_amount(29000000);
    input.set_fee(200000);
    input.set_sequence(1);
    input.set_account("rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF");
    input.set_destination("rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(hex(output.encoded()), "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc72337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb8337e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d");

    // invalid tag
    input.set_destination_tag(641505641505);

    ANY_SIGN(input, TWCoinTypeXRP);

    EXPECT_EQ(output.error(), Common::Proto::SigningError::Error_invalid_memo);
    EXPECT_EQ(output.encoded(), "");
}
