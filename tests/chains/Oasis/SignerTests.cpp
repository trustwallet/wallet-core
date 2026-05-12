// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Oasis/Address.h"
#include "Oasis/Signer.h"
#include "Oasis/Transaction.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Oasis::tests {

TEST(OasisSigner, Sign) {
    auto input = Proto::SigningInput();
    auto& transfer = *input.mutable_transfer();

    transfer.set_gas_price(0);
    transfer.set_gas_amount("0");
    transfer.set_nonce(0);
    transfer.set_to("oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5");
    transfer.set_amount("10000000");

    // The use of this context thing is explained here --> https://docs.oasis.dev/oasis-core/common-functionality/crypto#domain-separation
    transfer.set_context("oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4");

    auto key = parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0");
    input.set_private_key(key.data(), key.size());

    Proto::SigningOutput output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "a2697369676e6174757265a2697369676e617475726558406e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c6a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b73756e747275737465645f7261775f76616c7565585ea463666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680656e6f6e636500666d6574686f64707374616b696e672e5472616e73666572");
}

TEST(OasisSigner, EncodeVaruintZero) {
    // value == 0: all bytes zero, loop exits with i == data.size() == 32,
    // then data.begin() + 31 gives the last zero byte → [0x00].
    EXPECT_EQ(hex(encodeVaruint(0u)), "00");
}

TEST(OasisSigner, EncodeVaruintSmall) {
    // value == 10000000 (0x989680): first nonzero at index 29, sign byte at 28 → [0x00, 0x98, 0x96, 0x80].
    EXPECT_EQ(hex(encodeVaruint(10000000u)), "00989680");
}

TEST(OasisSigner, EncodeVaruintMSBSet) {
    // value with the high bit of the first significant byte set (0x80):
    // needs sign byte to avoid misreading as negative.
    EXPECT_EQ(hex(encodeVaruint(0x80u)), "0080");
}

TEST(OasisSigner, EncodeVaruintMaxByte) {
    // value == 2^248: data[0] == 0x01, i == 0 → sign byte must be prepended
    // (33-byte result). Before the fix, data.begin() - 1 was UB / a crash.
    uint256_t two_248 = uint256_t(1) << 248;
    auto encoded = encodeVaruint(two_248);
    ASSERT_EQ(encoded.size(), 33u);
    EXPECT_EQ(encoded[0], 0x00u);  // sign byte
    EXPECT_EQ(encoded[1], 0x01u);  // MSB of magnitude
    for (size_t j = 2; j < encoded.size(); ++j) {
        EXPECT_EQ(encoded[j], 0x00u) << "byte " << j << " should be zero";
    }
}

TEST(OasisSigner, EncodeVaruintAllOnes) {
    // value == 2^256 - 1 (all bits set): data[0] == 0xFF, i == 0 → sign byte prepended.
    uint256_t max = ~uint256_t(0);
    auto encoded = encodeVaruint(max);
    ASSERT_EQ(encoded.size(), 33u);
    EXPECT_EQ(encoded[0], 0x00u);
    for (size_t j = 1; j < encoded.size(); ++j) {
        EXPECT_EQ(encoded[j], 0xFFu) << "byte " << j << " should be 0xFF";
    }
}

} // namespace TW::Oasis::tests
