// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HDWallet.h"
#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <stdlib.h>
#include <string>

namespace TW::HDWalletInternalTests {

const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";

std::string nodeToHexString(const HDNode& node) {
    std::string s;
    s += std::to_string(node.depth);
    s += "-" + std::to_string(node.child_num);
    s += "--" + hex(data(node.chain_code, 32));
    s += "--" + hex(data(node.private_key, 32));
    s += "--" + hex(data(node.private_key_extension, 32));
    s += "--" + hex(data(node.public_key, 33));
    return s;
}

Data publicKeyFromPrivateKey(const Data& privateKey) {
    return PrivateKey(privateKey).getPublicKey(TWPublicKeyTypeSECP256k1).bytes;
}

TEST(HDWalletInternal, SquareDerivationRoutes) {
    /*
       Test 'square' derivation routes, result should be the same.
       Performing private derivation, then taking the public key yields the same as
       taking the public key first, and performing public derivation.
       This makes XPUB schemes possible.

         priv_node  --priv_deriv.-->  priv_child_node

             |                              |
          get_pub                        get_pub
             |                              |
             v                              v

          pub_node   ---pub_deriv.--->   pub_key

    */

    HDWallet wallet = HDWallet(mnemonic1, "");
    const auto derivationPath = DerivationPath("m/84'/0'/0'/1");
    const auto dpLastIndex = 2;
    const auto ExpectedFinalPublicKey = "02e0b4765e9012bfbbfe8c714f99beb681acc0a92bdb5acbf2f362c0aff986ad49";

    // getMasterNode
    auto masterNode = HDNode();
    hdnode_from_seed(wallet.getSeed().data(), HDWallet<>::mSeedSize, SECP256K1_NAME, &masterNode);

    auto node0 = masterNode;
    // getNode
    for (auto& index : derivationPath.indices) {
        hdnode_private_ckd(&node0, index.derivationIndex());
    }
    EXPECT_EQ(nodeToHexString(node0), "4-1--8423e869d887b6b0e7ca5f3bbed6e69234fb5d51aa02e9e8069fbffb2dc3c095--55f85b343359ec33aa3519a40673773d1f52677a044c7185529ce8f5fdb70625--0000000000000000000000000000000000000000000000000000000000000000--000000000000000000000000000000000000000000000000000000000000000000");

    {
        // Route 1 step 1: private node derivation
        auto node11 = node0;
        EXPECT_EQ(hdnode_private_ckd(&node11, dpLastIndex), 1);
        EXPECT_EQ(nodeToHexString(node11), "5-2--53aaec7a112524bc3c8c91b278ccb0cf7e322077dac6a832563eb33149bb0051--512503395481ea0c26fe341bc342c29f4a706be003d12179ec6b65aa8a8c352d--0000000000000000000000000000000000000000000000000000000000000000--000000000000000000000000000000000000000000000000000000000000000000");

        // Route 1 step 2: public key from private
        const auto publicKey = publicKeyFromPrivateKey(data(node11.private_key, 32));
        EXPECT_EQ(hex(publicKey), ExpectedFinalPublicKey);
    }

    {
        // Route 2 step 1: public node from private (extended public key)
        auto node21 = node0;
        const auto pub21 = publicKeyFromPrivateKey(data(node21.private_key, 32));
        ::memcpy(node21.public_key, pub21.data(), 33);
        EXPECT_EQ(nodeToHexString(node21), "4-1--8423e869d887b6b0e7ca5f3bbed6e69234fb5d51aa02e9e8069fbffb2dc3c095--55f85b343359ec33aa3519a40673773d1f52677a044c7185529ce8f5fdb70625--0000000000000000000000000000000000000000000000000000000000000000--026a940b5b683237037ecb230c402c5e351f38d41f00215e4d36006e9ff6b5cfba");

        // Route 2 step 2: public node derivation
        auto node22 = node21;
        EXPECT_EQ(hdnode_public_ckd(&node22, dpLastIndex), 1);
        EXPECT_EQ(nodeToHexString(node22), "5-2--53aaec7a112524bc3c8c91b278ccb0cf7e322077dac6a832563eb33149bb0051--0000000000000000000000000000000000000000000000000000000000000000--0000000000000000000000000000000000000000000000000000000000000000--02e0b4765e9012bfbbfe8c714f99beb681acc0a92bdb5acbf2f362c0aff986ad49");
        const auto publicKey = PublicKey(data(node22.public_key, 33), TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(publicKey.bytes), ExpectedFinalPublicKey);
    }
}

TEST(HDWalletInternal, PrivateAndPublicCkdDerivation) {
    /*

        PrivateKey1  ---->  PrivateKey2

             |                   |
             v                   v

         PublicKey1  ---->   PublicKey2

    */

    const auto PrivateKey1 = "55f85b343359ec33aa3519a40673773d1f52677a044c7185529ce8f5fdb70625";
    const auto PrivateKey2 = "512503395481ea0c26fe341bc342c29f4a706be003d12179ec6b65aa8a8c352d";
    const auto PublicKey1 = "026a940b5b683237037ecb230c402c5e351f38d41f00215e4d36006e9ff6b5cfba";
    const auto PublicKey2 = "02e0b4765e9012bfbbfe8c714f99beb681acc0a92bdb5acbf2f362c0aff986ad49";
    const auto ChainCode0 = "8423e869d887b6b0e7ca5f3bbed6e69234fb5d51aa02e9e8069fbffb2dc3c095";
    const auto dpIndex = 2;
    const auto curve = get_curve_by_name(SECP256K1_NAME);

    {   // PrivateKey1 -> PublicKey1
        EXPECT_EQ(hex(publicKeyFromPrivateKey(parse_hex(PrivateKey1))), PublicKey1);
    }
    {   // PrivateKey2 -> PublicKey2
        EXPECT_EQ(hex(publicKeyFromPrivateKey(parse_hex(PrivateKey2))), PublicKey2);
    }
    {   // PrivateKey1 -> PrivateKey2
        auto node = HDNode();
        node.depth = 4;
        node.child_num = 1;
        node.curve = curve;
        ::memcpy(node.chain_code, parse_hex(ChainCode0).data(), 32);
        ::memcpy(node.private_key, parse_hex(PrivateKey1).data(), 32);
        EXPECT_EQ(nodeToHexString(node), "4-1--8423e869d887b6b0e7ca5f3bbed6e69234fb5d51aa02e9e8069fbffb2dc3c095--55f85b343359ec33aa3519a40673773d1f52677a044c7185529ce8f5fdb70625--0000000000000000000000000000000000000000000000000000000000000000--000000000000000000000000000000000000000000000000000000000000000000");

        EXPECT_EQ(hdnode_private_ckd(&node, dpIndex), 1);

        EXPECT_EQ(nodeToHexString(node), "5-2--53aaec7a112524bc3c8c91b278ccb0cf7e322077dac6a832563eb33149bb0051--512503395481ea0c26fe341bc342c29f4a706be003d12179ec6b65aa8a8c352d--0000000000000000000000000000000000000000000000000000000000000000--000000000000000000000000000000000000000000000000000000000000000000");
        EXPECT_EQ(hex(data(node.private_key, 32)), PrivateKey2);
    }
    {   // PublicKey1 -> PublicKey2
        auto node = HDNode();
        node.depth = 4;
        node.child_num = 1;
        node.curve = curve;
        ::memcpy(node.chain_code, parse_hex(ChainCode0).data(), 32);
        ::memcpy(node.public_key, parse_hex(PublicKey1).data(), 33);
        EXPECT_EQ(nodeToHexString(node), "4-1--8423e869d887b6b0e7ca5f3bbed6e69234fb5d51aa02e9e8069fbffb2dc3c095--0000000000000000000000000000000000000000000000000000000000000000--0000000000000000000000000000000000000000000000000000000000000000--026a940b5b683237037ecb230c402c5e351f38d41f00215e4d36006e9ff6b5cfba");

        EXPECT_EQ(hdnode_public_ckd(&node, dpIndex), 1);

        EXPECT_EQ(nodeToHexString(node), "5-2--53aaec7a112524bc3c8c91b278ccb0cf7e322077dac6a832563eb33149bb0051--0000000000000000000000000000000000000000000000000000000000000000--0000000000000000000000000000000000000000000000000000000000000000--02e0b4765e9012bfbbfe8c714f99beb681acc0a92bdb5acbf2f362c0aff986ad49");
        EXPECT_EQ(hex(data(node.public_key, 33)), PublicKey2);
    }
}

} // namespace
