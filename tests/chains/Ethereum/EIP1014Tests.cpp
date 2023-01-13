// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <PrivateKey.h>
#include <HexCoding.h>
#include <Ethereum/EIP1014.h>
#include <Ethereum/AddressChecksum.h>
#include <TrustWalletCore/TWEthereumEip1014.h>
#include <TrustWalletCore/TWHash.h>
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {
    TEST(EthereumEip1014, Example0) {
        // C++
        {
            const std::string& from = "0x0000000000000000000000000000000000000000";
            const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
            Data initCodeHash = Hash::keccak256(parse_hex("0x00"));
            const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
            ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x4D1A2e2bB4F88F0250f26Ffff098B0b30B26BF38");
            ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0x4D1A2e2bB4F88F0250f26Ffff098B0b30B26BF38");
        }

        // C
        {
            const auto from = STRING("0x0000000000000000000000000000000000000000");
            const auto salt = DATA("0x0000000000000000000000000000000000000000000000000000000000000000");
            const auto initCodeHash = WRAPD(TWHashKeccak256(DATA("0x00").get()));
            const auto& result = WRAPS(TWEthereumEip1014AddressCreate2(from.get(), salt.get(), initCodeHash.get()));
            assertStringsEqual(result, "0x4D1A2e2bB4F88F0250f26Ffff098B0b30B26BF38");
        }
    }

    TEST(EthereumEip1014, Example1) {
        // C++
        {
            const std::string& from = "0xdeadbeef00000000000000000000000000000000";
            const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
            Data initCodeHash = Hash::keccak256(parse_hex("0x00"));
            const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
            ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0xB928f69Bb1D91Cd65274e3c79d8986362984fDA3");
            ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0xB928f69Bb1D91Cd65274e3c79d8986362984fDA3");
        }

        // C
        {
            const auto from = STRING("0xdeadbeef00000000000000000000000000000000");
            const auto salt = DATA("0x0000000000000000000000000000000000000000000000000000000000000000");
            const auto initCodeHash = WRAPD(TWHashKeccak256(DATA("0x00").get()));
            const auto& result = WRAPS(TWEthereumEip1014AddressCreate2(from.get(), salt.get(), initCodeHash.get()));
            assertStringsEqual(result, "0xB928f69Bb1D91Cd65274e3c79d8986362984fDA3");
        }
    }

    TEST(EthereumEip1014, Example2) {
        const std::string& from = "0xdeadbeef00000000000000000000000000000000";
        const Data salt = parse_hex("0x000000000000000000000000feed000000000000000000000000000000000000");
        Data initCodeHash = Hash::keccak256(parse_hex("0x00"));
        const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0xD04116cDd17beBE565EB2422F2497E06cC1C9833");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0xD04116cDd17beBE565EB2422F2497E06cC1C9833");
    }

    TEST(EthereumEip1014, Example3) {
        const std::string& from = "0x0000000000000000000000000000000000000000";
        const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
        Data initCodeHash = Hash::keccak256(parse_hex("0xdeadbeef"));
        const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x70f2b2914A2a4b783FaEFb75f459A580616Fcb5e");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0x70f2b2914A2a4b783FaEFb75f459A580616Fcb5e");
    }

    TEST(EthereumEip1014, Example4) {
        const std::string& from = "0x00000000000000000000000000000000deadbeef";
        const Data salt = parse_hex("0x00000000000000000000000000000000000000000000000000000000cafebabe");
        Data initCodeHash = Hash::keccak256(parse_hex("0xdeadbeef"));
        const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x60f3f640a8508fC6a86d45DF051962668E1e8AC7");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0x60f3f640a8508fC6a86d45DF051962668E1e8AC7");
    }

    TEST(EthereumEip1014, Example5) {
        const std::string& from = "0x00000000000000000000000000000000deadbeef";
        const Data salt = parse_hex("0x00000000000000000000000000000000000000000000000000000000cafebabe");
        Data initCodeHash = Hash::keccak256(parse_hex("0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"));
        const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0x1d8bfDC5D46DC4f61D6b6115972536eBE6A8854C");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0x1d8bfDC5D46DC4f61D6b6115972536eBE6A8854C");
    }

    TEST(EthereumEip1014, Example6) {
        const std::string& from = "0x0000000000000000000000000000000000000000";
        const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
        Data initCodeHash = Hash::keccak256(parse_hex("0x"));
        const auto& addressData = Ethereum::create2Address(from, salt, initCodeHash);
        ASSERT_EQ(Ethereum::checksumed(Ethereum::Address(hexEncoded(addressData))), "0xE33C0C7F7df4809055C3ebA6c09CFe4BaF1BD9e0");
        ASSERT_EQ(Ethereum::create2AddressString(from, salt, initCodeHash), "0xE33C0C7F7df4809055C3ebA6c09CFe4BaF1BD9e0");
    }
}
