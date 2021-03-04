//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CB58.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

TEST(CB58, decode) {
    Data decoded = CB58::avalanche.decodeCheck("32UWxgjUJd9s6Kyvxjj1u");
    EXPECT_EQ(data("Hello world"), decoded);
    decoded = CB58::avalanche.decodeCheck("2Eef9JnJ3Lz2c5op9");
    EXPECT_EQ(data("Ava Labs"), decoded);
    decoded = CB58::avalanche.decodeCheck("3agr4zJdC7C8GVmkgfkGLbcX7thwy5z3kqrq");
    EXPECT_EQ(data("THIS is a T3st addre55"), decoded);
    decoded = CB58::avalanche.decodeCheck("148vjpuxYXixb8DcbaWyeDE2fEG");
    Data original{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    EXPECT_EQ(original, decoded);
}

TEST(CB58, encode) {
    std::string encoded = CB58::avalanche.encodeCheck(data("Hello world"));
    EXPECT_EQ("32UWxgjUJd9s6Kyvxjj1u", encoded);
    encoded = CB58::avalanche.encodeCheck(data("Ava Labs"));
    EXPECT_EQ("2Eef9JnJ3Lz2c5op9", encoded);
    encoded = CB58::avalanche.encodeCheck(data("THIS is a T3st addre55"));
    EXPECT_EQ("3agr4zJdC7C8GVmkgfkGLbcX7thwy5z3kqrq", encoded);
    Data decoded{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    encoded = CB58::avalanche.encodeCheck(decoded);
    EXPECT_EQ("148vjpuxYXixb8DcbaWyeDE2fEG", encoded);
}
