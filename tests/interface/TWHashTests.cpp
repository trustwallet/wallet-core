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

const string brownFox = "The quick brown fox jumps over the lazy dog";
const string brownFoxDot = "The quick brown fox jumps over the lazy dog.";
const string hello = "hello";

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

TEST(TWHashTests, Sha1) {
    auto tests = {
        make_tuple(string(""), string("da39a3ee5e6b4b0d3255bfef95601890afd80709")),
        make_tuple(brownFox, string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12")),
        make_tuple(brownFoxDot, string("408d94384216f890ff7a0c3528e8bed1e0b01621")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA1(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Sha256) {
    auto tests = {
        make_tuple(string(""), string("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")),
        make_tuple(brownFox, string("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592")),
        make_tuple(brownFoxDot, string("ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA256(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Sha512) {
    auto tests = {
        make_tuple(string(""), string("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e")),
        make_tuple(brownFox, string("07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6")),
        make_tuple(brownFoxDot, string("91ea1245f20d46ae9a037a989f54f1f790f0a47607eeb8a14d12890cea77a1bbc6c7ed9cf205e67b7f2b8fd4c7dfd3a7a8617e45f3c463d481c7e586c39ac1ed")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA512(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Sha512_256) {
    auto tests = {
        make_tuple(string(""), string("c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a")),
        make_tuple(brownFox, string("dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d")),
        make_tuple(brownFoxDot, string("1546741840f8a492b959d9b8b2344b9b0eb51b004bba35c0aebaac86d45264c3")),
        make_tuple(hello, string("e30d87cfa2a75db545eac4d61baf970366a8357c7f72fa95b52d0accb698f13a")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA512_256(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Keccak256) {
    auto tests = {
        make_tuple(string(""), string("c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470")),
        make_tuple(brownFox, string("4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15")),
        make_tuple(brownFoxDot, string("578951e24efd62a3d63a86f7cd19aaa53c898fe287d2552133220370240b572d")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashKeccak256(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Keccak512) {
    auto tests = {
        make_tuple(string(""), string("0eab42de4c3ceb9235fc91acffe746b29c29a8c366b7c60e4e67c466f36a4304c00fa9caf9d87976ba469bcbe06713b435f091ef2769fb160cdab33d3670680e")),
        make_tuple(brownFox, string("d135bb84d0439dbac432247ee573a23ea7d3c9deb2a968eb31d47c4fb45f1ef4422d6c531b5b9bd6f449ebcc449ea94d0a8f05f62130fda612da53c79659f609")),
        make_tuple(brownFoxDot, string("ab7192d2b11f51c7dd744e7b3441febf397ca07bf812cceae122ca4ded6387889064f8db9230f173f6d1ab6e24b6e50f065b039f799f5592360a6558eb52d760")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashKeccak512(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, SHA3_256) {
    auto tests = {
        make_tuple(string(""), string("a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a")),
        make_tuple(brownFox, string("69070dda01975c8c120c3aada1b282394e7f032fa9cf32f4cb2259a0897dfc04")),
        make_tuple(brownFoxDot, string("a80f839cd4f83f6c3dafc87feae470045e4eb0d366397d5c6ce34ba1739f734d")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA3_256(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, SHA3_512) {
    auto tests = {
        make_tuple(string(""), string("a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26")),
        make_tuple(brownFox, string("01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450")),
        make_tuple(brownFoxDot, string("18f4f4bd419603f95538837003d9d254c26c23765565162247483f65c50303597bc9ce4d289f21d1c2f1f458828e33dc442100331b35e7eb031b5d38ba6460f8")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA3_512(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, RIPEMD) {
    auto tests = {
        make_tuple(string(""), string("9c1185a5c5e9fc54612808977ee8f548b2258d31")),
        make_tuple(brownFox, string("37f332f68db77bd9d7edd4969571ad671cf9dd3b")),
        make_tuple(brownFoxDot, string("fc850169b1f2ce72e3f8aa0aeb5ca87d6f8519c6")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashRIPEMD(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, Blake2b) {
    auto tests = {
        make_tuple(string(""), string("786a02f742015903c6c6fd852552d272912f4740e15847618a86e217f71f5419d25e1031afee585313896444934eb04b903a685b1448b755d56f701afe9be2ce")),
        make_tuple(brownFox, string("a8add4bdddfd93e4877d2746e62817b116364a1fa7bc148d95090bc7333b3673f82401cf7aa2e4cb1ecd90296e3f14cb5413f8ed77be73045b13914cdcd6a918")),
        make_tuple(brownFoxDot, string("87af9dc4afe5651b7aa89124b905fd214bf17c79af58610db86a0fb1e0194622a4e9d8e395b352223a8183b0d421c0994b98286cbf8c68a495902e0fe6e2bda2")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashBlake2b(inData.get(), 64));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, SHA256RIPEMD) {
    auto tests = {
        make_tuple(string(""), string("b472a266d0bd89c13706a4132ccfb16f7c3b9fcb")),
        make_tuple(brownFox, string("0e3397b4abc7a382b3ea2365883c3c7ca5f07600")),
        make_tuple(brownFoxDot, string("49f3b08d52a030415a5f7e84f72386e10acc4ebf")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA256RIPEMD(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, SHA256SHA256) {
    auto tests = {
        make_tuple(string(""), string("5df6e0e2761359d30a8275058e299fcc0381534545f55cf43e41983f5d4c9456")),
        make_tuple(brownFox, string("6d37795021e544d82b41850edf7aabab9a0ebe274e54a519840c4666f35b3937")),
        make_tuple(brownFoxDot, string("a51a910ecba8a599555b32133bf1829455d55fe576677b49cb561d874077385c")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashSHA256SHA256(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, XXHash64) {
    auto tests = {
        make_tuple(string(""), string("99e9d85137db46ef"), 0),
        make_tuple(brownFox, string("bc71da1f362d240b"), 0),
        make_tuple(brownFoxDot, string("73ad51577033ad44"), 0),
        make_tuple(string("123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"), 
            string("fd84b6962fcb8d09"), 0),
        make_tuple(string("123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"), 
            string("7ea0f7af4b4f9cf4"), 1),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashXXHash64(inData.get(), get<2>(test)));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}

TEST(TWHashTests, XXHash64Concat) {
    auto tests = {
        make_tuple(string(""), string("99e9d85137db46ef4bbea33613baafd5")),
        make_tuple(brownFox, string("bc71da1f362d240bdbc6d2dab69150df")),
        make_tuple(brownFoxDot, string("73ad51577033ad44269e8e5ef42d32d2")),
        make_tuple(string("Balances"), string("c2261276cc9d1f8598ea4b6a74b15c2f")),
        make_tuple(string("FreeBalance"), string("6482b9ade7bc6657aaca787ba1add3b4")),
    };
    for (auto &test: tests) {
        const auto inData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(get<0>(test).c_str()), get<0>(test).length()));
        const auto hash = WRAPD(TWHashTwoXXHash64Concat(inData.get()));
        EXPECT_EQ(hex(data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), get<1>(test));
    }
}
