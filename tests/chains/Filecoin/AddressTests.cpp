// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Filecoin/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Filecoin::tests {
// clang-format off

struct address_test {
    std::string encoded;
    uint8_t type;
    uint64_t actorID;
    const char* payloadHex;
};

static const address_test validAddresses[] = {
    // ID addresses
    {"f00",                    0, ""},
    {"f01",                    ""},
    {"f010",                   ""},
    {"f0150",                  ""},
    {"f0499",                  ""},
    {"f01024",                 ""},
    {"f01729",                 "c10d"},
    {"f0999999",               "bf843d"},
    {"f018446744073709551615", "ffffffffffffffffff01"},
    // secp256k1 addresses
    {"f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq", "ea0f0ea039b291a0f08fd179e0556a8c3277c0d3"},
    {"f12fiakbhe2gwd5cnmrenekasyn6v5tnaxaqizq6a", "d1500504e4d1ac3e89ac891a4502586fabd9b417"},
    {"f1wbxhu3ypkuo6eyp6hjx6davuelxaxrvwb2kuwva", "b06e7a6f0f551de261fe3a6fe182b422ee0bc6b6"},
    {"f1xtwapqc6nh4si2hcwpr3656iotzmlwumogqbuaa", "bcec07c05e69f92468e2b3e3bf77c874f2c5da8c"},
    {"f1xcbgdhkgkwht3hrrnui3jdopeejsoatkzmoltqy", "b882619d46558f3d9e316d11b48dcf211327026a"},
    {"f17uoq6tp427uzv7fztkbsnn64iwotfrristwpryy", "fd1d0f4dfcd7e99afcb99a8326b7dc459d32c628"},
    // Actor addresses
    {"f24vg6ut43yw2h2jqydgbg2xq7x6f4kub3bg6as6i", "e54dea4f9bc5b47d261819826d5e1fbf8bc5503b"},
    {"f25nml2cfbljvn4goqtclhifepvfnicv6g7mfmmvq", "eb58bd08a15a6ade19d0989674148fa95a8157c6"},
    {"f2nuqrg7vuysaue2pistjjnt3fadsdzvyuatqtfei", "6d21137eb4c4814269e894d296cf6500e43cd714"},
    {"f24dd4ox4c2vpf5vk5wkadgyyn6qtuvgcpxxon64a", "e0c7c75f82d55e5ed55db28033630df4274a984f"},
    {"f2gfvuyh7v2sx3patm5k23wdzmhyhtmqctasbr23y", "316b4c1ff5d4afb7826ceab5bb0f2c3e0f364053"},
    // BLS addresses
    {"f3vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss4a","ad58df696e2d4e91ea86c881e938ba4ea81b395e12797b84b9cf314b9546705e839c7a99d606b247ddb4f9ac7a3414dd"},
    {"f3wmuu6crofhqmm3v4enos73okk2l366ck6yc4owxwbdtkmpk42ohkqxfitcpa57pjdcftql4tojda2poeruwa","b3294f0a2e29e0c66ebc235d2fedca5697bf784af605c75af608e6a63d5cd38ea85ca8989e0efde9188b382f9372460d"},
    {"f3s2q2hzhkpiknjgmf4zq3ejab2rh62qbndueslmsdzervrhapxr7dftie4kpnpdiv2n6tvkr743ndhrsw6d3a","96a1a3e4ea7a14d49985e661b22401d44fed402d1d0925b243c923589c0fbc7e32cd04e29ed78d15d37d3aaa3fe6da33"},
    {"f3q22fijmmlckhl56rn5nkyamkph3mcfu5ed6dheq53c244hfmnq2i7efdma3cj5voxenwiummf2ajlsbxc65a","86b454258c589475f7d16f5aac018a79f6c1169d20fc33921dd8b5ce1cac6c348f90a3603624f6aeb91b64518c2e8095"},
    {"f3u5zgwa4ael3vuocgc5mfgygo4yuqocrntuuhcklf4xzg5tcaqwbyfabxetwtj4tsam3pbhnwghyhijr5mixa","a7726b038022f75a384617585360cee629070a2d9d28712965e5f26ecc40858382803724ed34f2720336f09db631f074"},
};

static const std::string invalidAddresses[] = {
    "",
    "f0",                 // Empty varuint
    "f0127"               // Short varuint
    "f01"                     // Long varuint
    "f00"                     // Long varuint
    "f0-1",                   // Negative :)
    "f018446744073709551616", // Greater than max uint64_t
    "f09223372036854775807",  // Overflow
    "f15ihq5ibzwki2b4ep2f46avlkr\0zhpqgtga7pdrq", // Embedded NUL
    "t15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Test net
    "a15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Unknown net
    "f95ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7pdrq",  // Unknown address type
    // Invalid checksum cases
    "f15ihq5ibzwki2b4ep2f46avlkrqzhpqgtga7rdrr",
    "f24vg6ut43yw2h2jqydgbg2xq7x6f4kub3bg6as66",
    "f3vvmn62lofvhjd2ugzca6sof2j2ubwok6cj4xxbfzz4yuxfkgobpihhd2thlanmsh3w2ptld2gqkn2jvlss44",
};

TEST(FilecoinAddress, IsValid) {
    for (const auto& test : validAddresses) {
        ASSERT_TRUE(Address::isValid(test.encoded))
            << "is_valid() != true: " << test.encoded << std::endl;
        // ASSERT_TRUE(Address::isValid(parse_hex(test.hex)))
        //     << "is_valid() != true: " << test.hex << std::endl;
    }
    for (const auto& address : invalidAddresses)
        ASSERT_FALSE(Address::isValid(address)) << "is_valid() != false: " << address << std::endl;

    // ASSERT_FALSE(Address::isValid(parse_hex("00"))) << "Empty varuint";
    // ASSERT_FALSE(Address::isValid(parse_hex("00ff"))) << "Short varuint";
    // ASSERT_FALSE(Address::isValid(parse_hex("00ff00ff"))) << "Varuint with hole";
    // ASSERT_FALSE(Address::isValid(parse_hex("000101"))) << "Long varuint";
    // ASSERT_FALSE(Address::isValid(parse_hex("000000"))) << "Long varuint";
    // ASSERT_FALSE(Address::isValid(parse_hex("00ffffffffffffffffff02"))) << "Overflow";
}

// TEST(FilecoinAddress, String) {
//     for (const auto& test : validAddresses) {
//         Address a(parse_hex(test.hex));
//         ASSERT_EQ(a.string(), test.encoded) << "Address(" << test.hex << ")";
//     }
// }

TEST(FilecoinAddress, FromString) {
    for (const auto& test : validAddresses) {
        Address a(test.encoded);
        ASSERT_EQ(hex(a.toBytes()), test.hex) << "Address(" << test.encoded << ")";
    }
}

}
