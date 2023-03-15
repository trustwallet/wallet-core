// Copyright © 2017-2023 Trust Wallet.
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
    for (auto& test : tests) {
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
    for (auto& test: tests) {
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

TEST(HashTests, allHashEnum) {
    const auto tests = {
        make_tuple(Hash::HasherSha1, "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"),
        make_tuple(Hash::HasherSha256, "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"),
        make_tuple(Hash::HasherSha512, "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6"),
        make_tuple(Hash::HasherSha512_256, "dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d"),
        make_tuple(Hash::HasherKeccak256, "4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15"),
        make_tuple(Hash::HasherKeccak512, "d135bb84d0439dbac432247ee573a23ea7d3c9deb2a968eb31d47c4fb45f1ef4422d6c531b5b9bd6f449ebcc449ea94d0a8f05f62130fda612da53c79659f609"),
        make_tuple(Hash::HasherSha3_256, "69070dda01975c8c120c3aada1b282394e7f032fa9cf32f4cb2259a0897dfc04"),
        make_tuple(Hash::HasherSha3_512, "01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450"),
        make_tuple(Hash::HasherRipemd, "37f332f68db77bd9d7edd4969571ad671cf9dd3b"),
        make_tuple(Hash::HasherBlake256, "7576698ee9cad30173080678e5965916adbb11cb5245d386bf1ffda1cb26c9d7"),
        make_tuple(Hash::HasherGroestl512, "badc1f70ccd69e0cf3760c3f93884289da84ec13c70b3d12a53a7a8a4a513f99715d46288f55e1dbf926e6d084a0538e4eebfc91cf2b21452921ccde9131718d"),
        make_tuple(Hash::HasherSha256d, "6d37795021e544d82b41850edf7aabab9a0ebe274e54a519840c4666f35b3937"),
        make_tuple(Hash::HasherSha256ripemd, "0e3397b4abc7a382b3ea2365883c3c7ca5f07600"),
        make_tuple(Hash::HasherSha3_256ripemd, "e70a0c74dd1b0c0d5af3c7ccbbe4b488d1b474b5"),
        make_tuple(Hash::HasherBlake256d, "4511ab8713d8d580cae73061345df903f603b99e7ec699ddae63c56eea200059"),
        make_tuple(Hash::HasherBlake256ripemd, "b4b44de1e854f7f3c0520b654204163f75f704e5"),
        make_tuple(Hash::HasherGroestl512d, "1209d229cfc9d7d6711369e2d7f369b0efc1459a9d407cbfc7daf4f54209347f2ee7e3e7522ba5d5ac4e7365445739919e23e2917baee10f23557f3d3fbc696d"),
    };

    for (auto& test: tests) {
        const auto hashFunc = get<0>(test);
        const auto expected = get<1>(test);
        EXPECT_EQ(hex(Hash::hash(hashFunc, brownFox)), expected);
    }
}

// More tests in TWHashTests
