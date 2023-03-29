// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWEthereumMessageSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <Ethereum/MessageSigner.h>
#include <HexCoding.h>
#include <PrivateKey.h>

#include <gtest/gtest.h>

namespace TW::Ethereum {
    TEST(EthereumEip712, SignMessageAndVerifyLegacy) {
        PrivateKey ethKey(parse_hex("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"));
        auto msg = R"(
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallet", "type": "address"}
                    ]
                },
                "primaryType": "Person",
                "domain": {
                    "name": "Ether Person",
                    "version": "1",
                    "chainId": 0,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "name": "Cow",
                    "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                }
            })";
        auto signature = Ethereum::MessageSigner::signTypedData(ethKey, msg, MessageType::Legacy);
        ASSERT_EQ(signature, "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf3761c");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(EthereumEip712, SignMessageAndVerifyEip155) {
        PrivateKey ethKey(parse_hex("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"));
        auto msg = R"(
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallet", "type": "address"}
                    ]
                },
                "primaryType": "Person",
                "domain": {
                    "name": "Ether Person",
                    "version": "1",
                    "chainId": 0,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "name": "Cow",
                    "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                }
            })";
        auto signature = Ethereum::MessageSigner::signTypedData(ethKey, msg, MessageType::Eip155, 0);
        ASSERT_EQ(signature, "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf37624");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(EthereumEip712, SignMessageAndVerifyInvalidEip155) {
        PrivateKey ethKey(parse_hex("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"));
        auto msg = R"(
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallet", "type": "address"}
                    ]
                },
                "primaryType": "Person",
                "domain": {
                    "name": "Ether Person",
                    "version": "1",
                    "chainId": 1,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "name": "Cow",
                    "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                }
            })";
        auto signature = Ethereum::MessageSigner::signTypedData(ethKey, msg, MessageType::Eip155, 0);
        ASSERT_EQ(signature, "EIP712 chainId is different than the current chainID.");
    }

    TEST(EthereumEip191, SignMessageAndVerifyLegacy) {
        PrivateKey ethKey(parse_hex("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"));
        auto msg = "Foo";
        auto signature = Ethereum::MessageSigner::signMessage(ethKey, msg, MessageType::Legacy);
        ASSERT_EQ(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(EthereumEip191, SignMessageAndVerifyEip155) {
        PrivateKey ethKey(parse_hex("03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d"));
        auto msg = "Foo";
        auto signature = Ethereum::MessageSigner::signMessage(ethKey, msg, MessageType::Eip155, 0);
        ASSERT_EQ(signature, "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(EthereumEip191, SignMessageAndVerifyImmutableX) {
        PrivateKey ethKey(parse_hex("3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84"));
        auto msg = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3";
        auto signature = Ethereum::MessageSigner::signMessage(ethKey, msg, MessageType::ImmutableX);
        ASSERT_EQ(signature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        auto pubKey = ethKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        ASSERT_TRUE(Ethereum::MessageSigner::verifyMessage(pubKey, msg, signature));
    }

    TEST(TWEthereumMessageSigner, SignAndVerifyImmutableX) {
        const auto privKeyData = "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3");

        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        const auto signature = WRAPS(TWEthereumMessageSignerSignMessageImmutableX(privateKey.get(), message.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));
    }

    TEST(TWEthereumMessageSigner, SignAndVerifyLegacy) {
        const auto privKeyData = "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Foo");

        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        const auto signature = WRAPS(TWEthereumMessageSignerSignMessage(privateKey.get(), message.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));
    }

    TEST(TWEthereumMessageSigner, SignAndVerifyEip155) {
        const auto privKeyData = "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        const auto message = STRING("Foo");

        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        const auto signature = WRAPS(TWEthereumMessageSignerSignMessageEip155(privateKey.get(), message.get(), 0));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey.get(), message.get(), signature.get()));
    }

    TEST(TWEthereumEip712, SignMessageAndVerifyLegacy) {
        const auto privKeyData = "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        auto msg = STRING(R"(
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallet", "type": "address"}
                    ]
                },
                "primaryType": "Person",
                "domain": {
                    "name": "Ether Person",
                    "version": "1",
                    "chainId": 0,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "name": "Cow",
                    "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                }
            })");
        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        const auto signature = WRAPS(TWEthereumMessageSignerSignTypedMessage(privateKey.get(), msg.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf3761c");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey.get(), msg.get(), signature.get()));
    }

    TEST(TWEthereumEip712, SignMessageAndVerifyEip155) {
        const auto privKeyData = "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d";
        const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(privKeyData).get()));
        auto msg = STRING(R"(
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallet", "type": "address"}
                    ]
                },
                "primaryType": "Person",
                "domain": {
                    "name": "Ether Person",
                    "version": "1",
                    "chainId": 0,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "name": "Cow",
                    "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
                }
            })");
        const auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeEthereum));
        const auto signature = WRAPS(TWEthereumMessageSignerSignTypedMessageEip155(privateKey.get(), msg.get(), 0));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(signature.get())), "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf37624");
        EXPECT_TRUE(TWEthereumMessageSignerVerifyMessage(pubKey.get(), msg.get(), signature.get()));
    }
}
