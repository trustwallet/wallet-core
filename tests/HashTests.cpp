// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

const string brownFox = "The quick brown fox jumps over the lazy dog";
const string brownFoxDot = "The quick brown fox jumps over the lazy dog.";

TEST(HashTests, Blake2b) {
    auto content = string("Hello world");
    auto hashed = Hash::blake2b(content, 64);
    auto result = hex(hashed);

    ASSERT_EQ(result, string("6ff843ba685842aa82031d3f53c48b66326df7639a63d128974c5c14f31a0f33343a8c65551134ed1ae0f2b0dd2bb495dc81039e3eeb0aa1bb0388bbeac29183"));
}

TEST(HashTests, Blake2bPersonal) {
    auto personal_string = string("MyApp Files Hash");
    auto personal_data = Data(personal_string.begin(), personal_string.end());
    auto content = string("the same content");
    auto hashed = Hash::blake2b(content, 32, personal_data);
    auto result = hex(hashed);

    ASSERT_EQ(result, string("20d9cd024d4fb086aae819a1432dd2466de12947831b75c5a30cf2676095d3b4"));
}

TEST(HashTests, Sha512_256) {
    auto tests = {
        make_tuple(string(""), string("c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a")),
        make_tuple(brownFox, string("dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d")),
        make_tuple(brownFoxDot, string("1546741840f8a492b959d9b8b2344b9b0eb51b004bba35c0aebaac86d45264c3")),
    };
    for (auto &test : tests) {
        auto hashed = Hash::sha512_256(get<0>(test));
        ASSERT_EQ(hex(hashed), get<1>(test));
    }
}

TEST(HashTests, Sha1) {
    auto tests = {
        make_tuple(string(""), string("da39a3ee5e6b4b0d3255bfef95601890afd80709")),
        make_tuple(brownFox, string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12")),
        make_tuple(brownFoxDot, string("408d94384216f890ff7a0c3528e8bed1e0b01621")),
    };
    for (auto &test: tests) {
        const auto hash = Hash::sha1(TW::data(get<0>(test)));
        EXPECT_EQ(hex(hash), get<1>(test));
    }
}

TEST(HashTests, hmac256) {
    const Data key = parse_hex("531cbfcf12a168faff61af28bf437377397b4bf435ee732cf4ac95761a651f14");
    const Data data = parse_hex("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224d");
    const auto expectedHmac = "a7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab";
    const Data hmac = Hash::hmac256(key, data);
    EXPECT_EQ(hex(hmac), expectedHmac);
}

// More tests in TWHashTests
