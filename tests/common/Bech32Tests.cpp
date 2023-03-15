// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;

struct DecodeTestData {
    std::string encoded;
    bool isValid;
    bool isValidM;
    std::string hrp;
    std::string dataHex;
};

std::vector<DecodeTestData> testData = {
    /// valid
    {"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", true, false, "bnb", "080301090f051414170f04160200111d0314131b1c1b1e041a080d091f1a1f06"},
    {"bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7k7grplx", true, false, "bc", "010e140f070d1a001912060b0d081504140311021d030c1d03040f1814060e1e160e140f070d1a001912060b0d081504140311021d030c1d03040f1814060e1e16"},
    {"bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y", false, true, "bc", "010e140f070d1a001912060b0d081504140311021d030c1d03040f1814060e1e160e140f070d1a001912060b0d081504140311021d030c1d03040f1814060e1e16"},
    /// invalid
    {"bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2", false, false, "", ""}, // 1-char diff

    /// valid test vectors (BIP173)
    {"A12UEL5L", true, false, "a", ""},
    {"a12uel5l", true, false, "a", ""},
    {"an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1tt5tgs", true, false, "an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio", ""},
    {"abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw", true, false, "abcdef", "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"},
    {"11qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqc8247j", true, false, "1", "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"},
    {"split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w", true, false, "split", "18171918161c01100b1d0819171d130d10171d16191c01100b03191d1b1903031d130b190303190d181d01190303190d"},
    {"?1ezyfcl", true, false, "?", ""},

    /// valid bech32m test vectors (BIP350)
    {"A1LQFN3A", false, true, "a", ""},
    {"a1lqfn3a", false, true, "a", ""},
    {"an83characterlonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11sg7hg6", false, true, "an83characterlonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber1", ""},
    {"abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx", false, true, "abcdef", "1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100"},
    {"11llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllludsr8", false, true, "1", "1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f"},
    {"split1checkupstagehandshakeupstreamerranterredcaperredlc445v", false, true, "split", "18171918161c01100b1d0819171d130d10171d16191c01100b03191d1b1903031d130b190303190d181d01190303190d"},
    {"?1v759aa", false, true, "?", ""},

    /// invalid test vectors (BIP173)
    {"\x20""1nwldj5", false, false, "", ""}, // HRP character out of range
    {"\x7F""1axkwrx", false, false, "", ""}, // HRP character out of range
    {"\x80""1eym55h", false, false, "", ""}, // HRP character out of range
    {"pzry9x0s0muk", false, false, "", ""}, // No separator character
    {"1pzry9x0s0muk", false, false, "", ""}, // Empty HRP
    {"x1b4n0q5v", false, false, "", ""}, // Invalid data character
    {"lt1igcx5c0", false, false, "", ""}, // Invalid data character
    {"li1dgmt3", false, false, "", ""}, // Too short checksum
    {"de1lg7wt""\xFF", false, false, "", ""}, // Invalid character in checksum
    {"A1G7SGD8", false, false, "", ""}, // checksum calculated with uppercase form of HRP
    {"10a06t8", false, false, "", ""}, // empty HRP
    {"1qzzfhee", false, false, "", ""}, // empty HRP

    /// invalid bech32m test vectors (BIP350)
    {"\x20""1xj0phk", false, false, "", ""}, // HRP character out of range
    {"\x7F""1g6xzxy", false, false, "", ""}, // HRP character out of range
    {"\x80""1vctc34", false, false, "", ""}, // HRP character out of range
    {"qyrz8wqd2c9m", false, false, "", ""}, // No separator character
    {"1qyrz8wqd2c9m", false, false, "", ""}, // Empty HRP
    {"y1b0jsk6g", false, false, "", ""}, // Invalid data character
    {"lt1igcx5c0", false, false, "", ""}, // Invalid data character
    {"in1muywd", false, false, "", ""}, // Too short checksum
    {"mm1crxm3i", false, false, "", ""}, // Invalid character in checksum
    {"au1s5cgom", false, false, "", ""}, // Invalid character in checksum
    {"M1VUXWEZ", false, false, "", ""}, // checksum calculated with uppercase form of HRP
    {"16plkw9", false, false, "", ""}, // empty HRP
    {"1p2gdwpf", false, false, "", ""}, // empty HRP
};

TEST(Bech32, decode) {
    for (auto& td: testData) {
        ASSERT_FALSE(td.isValid && td.isValidM); // a string cannot be valid under both

        auto res = Bech32::decode(td.encoded);
        if (!td.isValid && !td.isValidM) {
            EXPECT_EQ(std::get<0>(res), "");
            EXPECT_EQ(std::get<1>(res).size(), 0ul);
        } else {
            if (td.isValid) {
                EXPECT_EQ(std::get<2>(res), Bech32::ChecksumVariant::Bech32);
            } else if (td.isValidM) {
                EXPECT_EQ(std::get<2>(res), Bech32::ChecksumVariant::Bech32M);
            }
            EXPECT_EQ(std::get<0>(res), td.hrp) << "Wrong hrp for " << td.encoded;
            EXPECT_EQ(hex(std::get<1>(res)), td.dataHex) << "Wrong data for " << td.encoded;
        }
    }
}

TEST(Bech32, encode) {
    for (auto& td: testData) {
        if (!td.isValid) {
            continue;
        }
        auto res = Bech32::encode(td.hrp, parse_hex(td.dataHex), Bech32::ChecksumVariant::Bech32);
        std::string encodedLow = td.encoded;
        std::transform(encodedLow.begin(), encodedLow.end(), encodedLow.begin(), [](unsigned char c){ return std::tolower(c); });
        EXPECT_EQ(res, encodedLow);
    }
}

TEST(Bech32, encodeM) {
    for (auto& td: testData) {
        if (!td.isValidM) {
            continue;
        }
        auto res = Bech32::encode(td.hrp, parse_hex(td.dataHex), Bech32::ChecksumVariant::Bech32M);
        std::string encodedLow = td.encoded;
        std::transform(encodedLow.begin(), encodedLow.end(), encodedLow.begin(), [](unsigned char c){ return std::tolower(c); });
        EXPECT_EQ(res, encodedLow);
    }
}
