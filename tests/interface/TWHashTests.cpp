// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "HexCoding.h"

#include <TrustWalletCore/TWHash.h>

#include "TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace std;
using namespace TW;

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
        make_tuple(string("The quick brown fox jumps over the lazy dog"), string("dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d")),
        make_tuple(string("The quick brown fox jumps over the lazy dog."), string("1546741840f8a492b959d9b8b2344b9b0eb51b004bba35c0aebaac86d45264c3")),
    };

    for (auto &test : tests) {
        auto hashed = Hash::sha512_256(get<0>(test));
        ASSERT_EQ(hex(hashed), get<1>(test));
    }
}

TEST(TWHashTests, Sha512_256) {
    auto message = std::string("hello");
    auto messageData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(message.data()), message.size()));
    auto hashed = WRAPD(TWHashSHA512_256(messageData.get()));
    auto expectedString = WRAPS(TWStringCreateWithUTF8Bytes("e30d87cfa2a75db545eac4d61baf970366a8357c7f72fa95b52d0accb698f13a"));
    auto expected = WRAPD(TWDataCreateWithHexString(expectedString.get()));

    ASSERT_TRUE(TWDataEqual(hashed.get(), expected.get()));
}

TEST(TWHashTests, XXHash64) {
    auto message = std::string("123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF");
    auto message2 = std::string("123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF");
    auto messageData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(message.data()), message.size()));
    auto messageData2 = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(message2.data()), message2.size()));
    auto hashed = WRAPD(TWHashXXHash64(messageData.get(), 0));
    auto hashed2 = WRAPD(TWHashXXHash64(messageData2.get(), 1));

    auto result = dataFromTWData(hashed.get());
    auto result2 = dataFromTWData(hashed2.get());

    ASSERT_EQ(hex(*result), string("fd84b6962fcb8d09"));
    ASSERT_EQ(hex(*result2), string("7ea0f7af4b4f9cf4"));
}

TEST(TWHashTests, TwoXXHash64Concat) {
    auto message = std::string("Balances");
    auto message2 = std::string("FreeBalance");
    auto messageData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(message.data()), message.size()));
    auto messageData2 = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(message2.data()), message2.size()));
    auto hashed = WRAPD(TWHashTwoXXHash64Concat(messageData.get()));
    auto hashed2 = WRAPD(TWHashTwoXXHash64Concat(messageData2.get()));

    auto result = dataFromTWData(hashed.get());
    auto result2 = dataFromTWData(hashed2.get());

    ASSERT_EQ(hex(*result), string("c2261276cc9d1f8598ea4b6a74b15c2f"));
    ASSERT_EQ(hex(*result2), string("6482b9ade7bc6657aaca787ba1add3b4"));
}
