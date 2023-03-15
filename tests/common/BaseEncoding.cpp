// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base32.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Base32::tests {

void TestBase32Encode(const char* decoded_hex, const char* expected_encoded_in, const char* alphabet_in = nullptr) {
    auto decoded = parse_hex(std::string(decoded_hex));
    auto encoded = encode(decoded, alphabet_in);
    ASSERT_EQ(std::string(expected_encoded_in), encoded);
}

void TestBase32Decode(const char* encoded_in, const char* expected_decoded_hex, const char* alphabet_in = nullptr) {
    Data decoded;
    bool res = decode(std::string(encoded_in), decoded, alphabet_in);
    ASSERT_TRUE(res);
    auto decoded_hex = hex(decoded);
    ASSERT_EQ(std::string(expected_decoded_hex), decoded_hex);
}

TEST(Base32, Encode) {
    TestBase32Encode("01", "AE");
    TestBase32Encode("010203", "AEBAG");
    TestBase32Encode("", "");
    TestBase32Encode(
        "48450c2745890def7da06fc2551f912a14f9fc581c12db6e4d6f73f2fd0b2ad50df3d396",
        "JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TSY");
    TestBase32Encode(
        "3dd160d60673bd9b13adc25dad5d988d0d9f4ccdbe95a2122f9ef28b3ce4e89693074620",
        "HXIWBVQGOO6ZWE5NYJO22XMYRUGZ6TGNX2K2EERPT3ZIWPHE5CLJGB2GEA");
    TestBase32Encode(
        "7a263717bf89dd55cf7f533e315c6224d635edc0e88206fd70e733bf2cc516be84e910f7",
        "PITDOF57RHOVLT37KM7DCXDCETLDL3OA5CBAN7LQ44Z36LGFC27IJ2IQ64");
}

TEST(Base32, Decode) {
    TestBase32Decode("AE", "01");
    TestBase32Decode("AEBAG", "010203");
    TestBase32Decode("", "");
    TestBase32Decode(
        "JBCQYJ2FREG667NAN7BFKH4RFIKPT7CYDQJNW3SNN5Z7F7ILFLKQ346TSY",
        "48450c2745890def7da06fc2551f912a14f9fc581c12db6e4d6f73f2fd0b2ad50df3d396");
    TestBase32Decode(
        "HXIWBVQGOO6ZWE5NYJO22XMYRUGZ6TGNX2K2EERPT3ZIWPHE5CLJGB2GEA",
        "3dd160d60673bd9b13adc25dad5d988d0d9f4ccdbe95a2122f9ef28b3ce4e89693074620");
    TestBase32Decode(
        "PITDOF57RHOVLT37KM7DCXDCETLDL3OA5CBAN7LQ44Z36LGFC27IJ2IQ64",
        "7a263717bf89dd55cf7f533e315c6224d635edc0e88206fd70e733bf2cc516be84e910f7");
}

TEST(Base32, EncodeNimiq) {
    const char* BASE32_ALPHABET_NIMIQ = "0123456789ABCDEFGHJKLMNPQRSTUVXY";
    TestBase32Encode("01", "04", BASE32_ALPHABET_NIMIQ);
    TestBase32Encode("010203", "04106", BASE32_ALPHABET_NIMIQ);
}

TEST(Base32, DecodeInvalid) {
    Data decoded;
    ASSERT_FALSE(decode("+-", decoded));  // invalid characters
    ASSERT_FALSE(decode("A", decoded));   // invalid odd length
    ASSERT_FALSE(decode("ABC", decoded)); // invalid odd length
}

} // namespace TW::Base32::tests
