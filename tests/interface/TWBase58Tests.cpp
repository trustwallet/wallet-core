// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBase58.h>

#include <gtest/gtest.h>

TEST(TWBase58, Encode) {
    const auto input = DATA("00769bdff96a02f9135a1d19b749db6a78fe07dc90");
    auto result = WRAPS(TWBase58Encode(input.get()));
    assertStringsEqual(result, "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
}

TEST(TWBase58, EncodeNoCheck) {
    const auto input = DATA("00769bdff96a02f9135a1d19b749db6a78fe07dc90c3507da5");
    auto result = WRAPS(TWBase58EncodeNoCheck(input.get()));
    assertStringsEqual(result, "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
}

TEST(TWBase58, EncodeNoCheck2) {
    const auto input = DATA("00769bdff96a02f9135a1d19b749db6a78fe07dc90deadbeef");
    auto result = WRAPS(TWBase58EncodeNoCheck(input.get()));
    assertStringsEqual(result, "1Bp9U1ogV3A14FMvKbRJms7ctyso5FdSz2");
}

TEST(TWBase58, Decode) {
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    auto result = WRAPD(TWBase58Decode(input.get()));
    assertHexEqual(result, "00769bdff96a02f9135a1d19b749db6a78fe07dc90");
}

TEST(TWBase58, DecodeNoCheck) {
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx");
    auto result = WRAPD(TWBase58DecodeNoCheck(input.get()));
    assertHexEqual(result, "00769bdff96a02f9135a1d19b749db6a78fe07dc90c3507da5");
}

TEST(TWBase58, Decode_WrongChecksum) {
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso5FdSz2");
    auto result = WRAPD(TWBase58Decode(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}

TEST(TWBase58, DecodeNoCheck_WrongChecksum) {
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso5FdSz2");
    auto result = WRAPD(TWBase58DecodeNoCheck(input.get()));
    // decodes despite wrong checksum
    assertHexEqual(result, "00769bdff96a02f9135a1d19b749db6a78fe07dc90deadbeef");
}

TEST(TWBase58, Decode_InvalidChar) {
    // 0 is invalid
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tc0");
    auto result = WRAPD(TWBase58Decode(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}

TEST(TWBase58, DecodeNoCheck_InvalidChar) {
    // 0 is invalid
    const auto input = STRING("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tc0");
    auto result = WRAPD(TWBase58DecodeNoCheck(input.get()));
    ASSERT_EQ(result.get(), nullptr);
}
