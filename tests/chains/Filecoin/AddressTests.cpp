// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Filecoin/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Filecoin::tests {
// clang-format off

struct address_test {
    std::string string;
    std::string encoded;
    uint64_t actorID;
    std::string payloadHex;
};

static const address_test validAddresses[] = {
    // ID addresses
    {"f00",                    "0000",                   0,                     ""},
    {"f01",                    "0001",                   1,                     ""},
    {"f010",                   "000a",                   10,                    ""},
    {"f0150",                  "009601",                 150,                   ""},
    {"f0499",                  "00f303",                 499,                   ""},
    {"f01024",                 "008008",                 1024,                  ""},
    {"f01729",                 "00c10d",                 1729,                  ""},
    {"f0999999",               "00bf843d",               999999,                ""},
    {"f018446744073709551615", "00ffffffffffffffffff01", 18446744073709551615U, ""},
    // secp256k1 addresses
    {"f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq", "01ea0f0ea039b291a0f08fd179e0556a8c3277c0d3",  0, "ea0f0ea039b291a0f08fd179e0556a8c3277c0d3"},
    {"f12fiakbhe2gwd5cnmrenekasyn6v5tnaxaqizq6a", "01d1500504e4d1ac3e89ac891a4502586fabd9b417", 0, "d1500504e4d1ac3e89ac891a4502586fabd9b417"},
    {"f1wbxhu3ypkuo6eyp6hjx6davuelxaxrvwb2kuwva", "01b06e7a6f0f551de261fe3a6fe182b422ee0bc6b6", 0, "b06e7a6f0f551de261fe3a6fe182b422ee0bc6b6"},
    {"f1xtwapqc6nh4si2hcwpr3656iotzmlwumogqbuaa", "01bcec07c05e69f92468e2b3e3bf77c874f2c5da8c", 0, "bcec07c05e69f92468e2b3e3bf77c874f2c5da8c"},
    {"f1xcbgdhkgkwht3hrrnui3jdopeejsoatkzmoltqy", "01b882619d46558f3d9e316d11b48dcf211327026a", 0, "b882619d46558f3d9e316d11b48dcf211327026a"},
    {"f17uoq6tp427uzv7fztkbsnn64iwotfrristwpryy", "01fd1d0f4dfcd7e99afcb99a8326b7dc459d32c628", 0, "fd1d0f4dfcd7e99afcb99a8326b7dc459d32c628"},
    // Actor addresses
    {"f24vg6ut43yw2h2jqydgbg2xq7x6f4kub3bg6as6i", "02e54dea4f9bc5b47d261819826d5e1fbf8bc5503b", 0, "e54dea4f9bc5b47d261819826d5e1fbf8bc5503b"},
    {"f25nml2cfbljvn4goqtclhifepvfnicv6g7mfmmvq", "02eb58bd08a15a6ade19d0989674148fa95a8157c6", 0, "eb58bd08a15a6ade19d0989674148fa95a8157c6"},
    {"f2nuqrg7vuysaue2pistjjnt3fadsdzvyuatqtfei", "026d21137eb4c4814269e894d296cf6500e43cd714", 0, "6d21137eb4c4814269e894d296cf6500e43cd714"},
    {"f24dd4ox4c2vpf5vk5wkadgyyn6qtuvgcpxxon64a", "02e0c7c75f82d55e5ed55db28033630df4274a984f", 0, "e0c7c75f82d55e5ed55db28033630df4274a984f"},
    {"f2gfvuyh7v2sx3patm5k23wdzmhyhtmqctasbr23y", "02316b4c1ff5d4afb7826ceab5bb0f2c3e0f364053", 0, "316b4c1ff5d4afb7826ceab5bb0f2c3e0f364053"},
    // BLS addresses
    {"f3vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss4a","03ad58df696e2d4e91ea86c881e938ba4ea81b395e12797b84b9cf314b9546705e839c7a99d606b247ddb4f9ac7a3414dd", 0, "ad58df696e2d4e91ea86c881e938ba4ea81b395e12797b84b9cf314b9546705e839c7a99d606b247ddb4f9ac7a3414dd"},
    {"f3wmuu6crofhqmm3v4enos73okk2l366ck6yc4owxwbdtkmpk42ohkqxfitcpa57pjdcftql4tojda2poeruwa","03b3294f0a2e29e0c66ebc235d2fedca5697bf784af605c75af608e6a63d5cd38ea85ca8989e0efde9188b382f9372460d", 0, "b3294f0a2e29e0c66ebc235d2fedca5697bf784af605c75af608e6a63d5cd38ea85ca8989e0efde9188b382f9372460d"},
    {"f3s2q2hzhkpiknjgmf4zq3ejab2rh62qbndueslmsdzervrhapxr7dftie4kpnpdiv2n6tvkr743ndhrsw6d3a","0396a1a3e4ea7a14d49985e661b22401d44fed402d1d0925b243c923589c0fbc7e32cd04e29ed78d15d37d3aaa3fe6da33", 0, "96a1a3e4ea7a14d49985e661b22401d44fed402d1d0925b243c923589c0fbc7e32cd04e29ed78d15d37d3aaa3fe6da33"},
    {"f3q22fijmmlckhl56rn5nkyamkph3mcfu5ed6dheq53c244hfmnq2i7efdma3cj5voxenwiummf2ajlsbxc65a","0386b454258c589475f7d16f5aac018a79f6c1169d20fc33921dd8b5ce1cac6c348f90a3603624f6aeb91b64518c2e8095", 0, "86b454258c589475f7d16f5aac018a79f6c1169d20fc33921dd8b5ce1cac6c348f90a3603624f6aeb91b64518c2e8095"},
    {"f3u5zgwa4ael3vuocgc5mfgygo4yuqocrntuuhcklf4xzg5tcaqwbyfabxetwtj4tsam3pbhnwghyhijr5mixa","03a7726b038022f75a384617585360cee629070a2d9d28712965e5f26ecc40858382803724ed34f2720336f09db631f074", 0, "a7726b038022f75a384617585360cee629070a2d9d28712965e5f26ecc40858382803724ed34f2720336f09db631f074"},
    // Delegated addresses
    {"f432f77777777x32lpna",                         "0420ffffffffff",                               32,                    "ffffffffff"},
    {"f418446744073709551615ftnkyfaq",               "04ffffffffffffffffff01",                       18446744073709551615U, ""},
    {"f410frw6wy7w6sbsguyn3yzeygg34fgf72n5ao5sxyky", "040a8dbd6c7ede90646a61bbc649831b7c298bfd37a0", 10,                    "8dbd6c7ede90646a61bbc649831b7c298bfd37a0"},
    {"f410f2oekwcmo2pueydmaq53eic2i62crtbeyuzx2gmy", "040ad388ab098ed3e84c0d808776440b48f685198498", 10,                    "d388ab098ed3e84c0d808776440b48f685198498"},
    {"f418446744073709551615faaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafbbuagu","04ffffffffffffffffff01000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 18446744073709551615U, "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"},
};

static const std::string invalidAddresses[] = {
    "",
    "f0-1",                   // Negative :)
    "f018446744073709551616", // Greater than max uint64_t
    "f418446744073709551615", // No "f" separator
    "f4f77777777vnmsana",     // Empty Actor ID
    "f15ihq5ibzwki2b4ep2f46avlkr\0zhpqgtga7pdrq", // Embedded NUL
    "t15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Test net
    "a15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Unknown net
    "f95ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Unknown address type
    // Invalid checksum cases
    "f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7rdrr",
    "f24vg6ut43yw2h2jqydgbg2xq7x6f4kub3bg6as66",
    "f3vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss44",
    "f0vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss44",
    "f410f2oekwcmo2pueydmaq53eic2i62crtbeyuzx2gma",
};

TEST(FilecoinAddress, IsValid) {
    for (auto&& test : validAddresses) {
        ASSERT_TRUE(Address::isValid(test.string))
            << "isValid(string) != true: " << test.string;

        const Data encodedBytes = parse_hex(test.encoded);
        ASSERT_TRUE(Address::isValid(encodedBytes))
            << "isValid(Data) != true: " << test.encoded;
    }
}

TEST(FilecoinAddress, IsInvalid) {
    for (auto&& address : invalidAddresses) {
        ASSERT_FALSE(Address::isValid(address))
            << "isValid(string) != false: " << address;
    }

    ASSERT_FALSE(Address::isValid(parse_hex("00"))) << "Empty varuint";
    ASSERT_FALSE(Address::isValid(parse_hex("00ff"))) << "Short varuint";
    ASSERT_FALSE(Address::isValid(parse_hex("00ff00ff"))) << "Varuint with hole";
    ASSERT_FALSE(Address::isValid(parse_hex("000101"))) << "Long varuint";
    ASSERT_FALSE(Address::isValid(parse_hex("000000"))) << "Long varuint";
    ASSERT_FALSE(Address::isValid(parse_hex("00ffffffffffffffffff80"))) << "Overflow";
}

TEST(FilecoinAddress, Equal) {
    for (auto&& test : validAddresses) {
        const Data encodedBytes = parse_hex(test.encoded);
        const Address lhs(test.string), rhs(encodedBytes);
        ASSERT_EQ(lhs, rhs) << "Address(string) != Address(Data)";
    }

    EXPECT_ANY_THROW(new Address(Data{}));
}

TEST(FilecoinAddress, ExpectedProperties) {
    for (auto&& test : validAddresses) {
        const Data encodedBytes = parse_hex(test.encoded);
        const uint8_t expectedType = encodedBytes[0];

        const Address address(encodedBytes);

        const auto actualType = static_cast<uint8_t>(address.type);
        ASSERT_TRUE(actualType == expectedType)
            << "Unexpected type: " << actualType << " != " << expectedType << ": " << test.string;
        ASSERT_TRUE(address.actorID == test.actorID)
            << "Unexpected actorID: " << address.actorID << " != " << test.actorID << ": " << test.string;
        ASSERT_TRUE(address.payload == parse_hex(test.payloadHex))
            << "Unexpected payload: " << hex(address.payload) << " != " << test.payloadHex;
    }
}

TEST(FilecoinAddress, ToString) {
    for (auto&& test : validAddresses) {
        Address a(parse_hex(test.encoded));
        ASSERT_EQ(a.string(), test.string) << "Address(" << test.encoded << ")";
    }
}

TEST(FilecoinAddress, ToBytes) {
    for (auto&& test : validAddresses) {
        Address a(test.string);
        ASSERT_EQ(hex(a.toBytes()), test.encoded) << "Address(" << test.string << ")";
    }

    for (const auto& test : invalidAddresses)
        EXPECT_ANY_THROW(new Address(test));
}

} // TW::Filecoin::tests
