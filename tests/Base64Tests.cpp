// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Base64;

TEST(Base64, encode) {
    auto encoded = encode(data("Hello, world!"));
    EXPECT_EQ("SGVsbG8sIHdvcmxkIQ==", encoded);
    encoded = encode(data("1"));
    EXPECT_EQ("MQ==", encoded);
    encoded = encode(data("12"));
    EXPECT_EQ("MTI=", encoded);
    encoded = encode(data("123"));
    EXPECT_EQ("MTIz", encoded);
    encoded = encode(data("1234"));
    EXPECT_EQ("MTIzNA==", encoded);
    encoded = encode(data(""));
    EXPECT_EQ("", encoded);
    encoded = encode(data("Lorem ipsum dolor sit amet, consectetur adipiscing elit"));
    EXPECT_EQ("TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdA==", encoded);
    encoded = encode(parse_hex("11ff8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d291b"));
    EXPECT_EQ("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb", encoded);
}

TEST(Base64, decode) {
    auto decoded = decode("SGVsbG8sIHdvcmxkIQ==");
    EXPECT_EQ(hex(data("Hello, world!")), hex(decoded));
    decoded = decode("MQ==");
    EXPECT_EQ(hex(data("1")), hex(decoded));
    decoded = decode("MTI=");
    EXPECT_EQ(hex(data("12")), hex(decoded));
    decoded = decode("MTIz");
    EXPECT_EQ(hex(data("123")), hex(decoded));
    decoded = decode("");
    EXPECT_EQ("", hex(decoded));
    decoded = decode("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb");
    EXPECT_EQ("11ff8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d291b", hex(decoded));
}

TEST(Base64, UrlFormat) {
    const std::string const1 = "11003faa8556289975ec991ac9994dfb613abec4ea000d5094e6379080f594e559b330b8";
    
    // Encoded string has both special characters
    auto encoded = encode(parse_hex(const1));
    EXPECT_EQ("EQA/qoVWKJl17JkayZlN+2E6vsTqAA1QlOY3kID1lOVZszC4", encoded);
    encoded = encodeBase64Url(parse_hex(const1));
    EXPECT_EQ("EQA_qoVWKJl17JkayZlN-2E6vsTqAA1QlOY3kID1lOVZszC4", encoded);

    auto decoded = decode("EQA/qoVWKJl17JkayZlN+2E6vsTqAA1QlOY3kID1lOVZszC4");
    EXPECT_EQ(const1, hex(decoded));
    decoded = decodeBase64Url("EQA_qoVWKJl17JkayZlN-2E6vsTqAA1QlOY3kID1lOVZszC4");
    EXPECT_EQ(const1, hex(decoded));
}
