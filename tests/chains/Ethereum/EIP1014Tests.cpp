// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <PrivateKey.h>
#include <HexCoding.h>
#include <Ethereum/EIP1014.h>
#include <Ethereum/AddressChecksum.h>
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {
    TEST(EthereumEip1014, Example0) {
        const std::string& from = "0x0000000000000000000000000000000000000000";
        const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
        Data initCode = salt;
        const auto& addressData = Ethereum::create2Address(from, salt, initCode);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0xFFc4f52f884a02bCd5716744cD622127366F2edf");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCode), "0xFFc4f52f884a02bCd5716744cD622127366F2edf");
    }

    TEST(EthereumEip1014, Example1) {
        const std::string& from = "0xdeadbeef00000000000000000000000000000000";
        const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
        Data initCode = salt;
        const auto& addressData = Ethereum::create2Address(from, salt, initCode);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x85f15e045e1244aC03289B48448249DC0A34AA30");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCode), "0x85f15e045e1244aC03289B48448249DC0A34AA30");
    }

    TEST(EthereumEip1014, Example2) {
        const std::string& from = "0xdeadbeef00000000000000000000000000000000";
        const Data salt = parse_hex("0x000000000000000000000000feed000000000000000000000000000000000000");
        Data initCode = parse_hex("0x00");
        initCode.resize(32);
        const auto& addressData = Ethereum::create2Address(from, salt, initCode);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x2DB27D1d6BE32C9abfA484BA3d591101881D4B9f");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCode), "0x2DB27D1d6BE32C9abfA484BA3d591101881D4B9f");
    }

    TEST(EthereumEip1014, Example3) {
        const std::string& from = "0x0000000000000000000000000000000000000000";
        const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
        Data initCode = parse_hex("0xdeadbeef");
        initCode.resize(32);
        const auto& addressData = Ethereum::create2Address(from, salt, initCode);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x219438aC82230Cb9A9C13Cd99D324fA1d66CF018");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCode), "0x219438aC82230Cb9A9C13Cd99D324fA1d66CF018");
    }

    TEST(EthereumEip1014, Example4) {
        const std::string& from = "0x00000000000000000000000000000000deadbeef";
        const Data salt = parse_hex("0x00000000000000000000000000000000000000000000000000000000cafebabe");
        Data initCode = parse_hex("0xdeadbeef");
        initCode.resize(32);
        const auto& addressData = Ethereum::create2Address(from, salt, initCode);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0xa42778FAd882048F0974F3dcef8969669269FCE2");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCode), "0xa42778FAd882048F0974F3dcef8969669269FCE2");
    }
}
