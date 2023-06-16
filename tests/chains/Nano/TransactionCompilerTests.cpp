// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "TransactionCompiler.h"
#include "proto/Nano.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStellarPassphrase.h>

#include "HexCoding.h"
#include "Nano/Signer.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include "TestAccounts.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Nano::tests {

TEST(NanoCompiler, CompileWithSignatures) {
    /// Step 1 : Prepare transaction input(protobuf)
    auto coin = TWCoinTypeNano;
    const auto privateKey = PrivateKey(parse_hex(kPrivateKey));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Blake2b);
    const auto linkBlock = parse_hex("491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507");

    auto input = Proto::SigningInput();
    input.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
    input.set_link_block(linkBlock.data(), linkBlock.size());
    input.set_representative(kRepOfficial1);
    input.set_balance("96242336390000000000000000000");
    auto inputString = input.SerializeAsString();
    auto localInputData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashData = TransactionCompiler::preImageHashes(coin, localInputData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    ASSERT_TRUE(
        preSigningOutput.ParseFromArray(preImageHashData.data(), (int)preImageHashData.size()));
    ASSERT_EQ(Common::Proto::OK, preSigningOutput.error());

    auto preImage = data(preSigningOutput.data());
    auto preImageHash = data(preSigningOutput.data_hash());
    const auto ExpectedPreImageHash = "f9a323153daefe041efb94d69b9669c882c935530ed953bbe8a665dfedda9696";
    ASSERT_EQ(ExpectedPreImageHash, hex(preImageHash));
    // Verify signature (pubkey & hash & signature)
    Data signature = parse_hex("d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09");
    EXPECT_TRUE(publicKey.verify(signature, preImageHash));
    /// Step 3 : Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, localInputData, {signature}, {publicKey.bytes});
    const auto ExpectedOutJson = "{\"account\":\"nano_1bhbsc9yuh15anq3owu1izw1nk7bhhqefrkhfo954fyt8dk1q911buk1kk4c\","
                                 "\"balance\":\"96242336390000000000000000000\","
                                 "\"link\":\"491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507\","
                                 "\"link_as_account\":\"nano_1kazsap8mc481zbqbcqjytpf9mmigj87qr5k5fhf97579t4k8fa94octjx6d\","
                                 "\"previous\":\"0000000000000000000000000000000000000000000000000000000000000000\","
                                 "\"representative\":\"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4\","
                                 "\"signature\":\"d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09\","
                                 "\"type\":\"state\"}";
    {
        TW::Nano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(hex(signature), hex(output.signature()));
        EXPECT_EQ(ExpectedPreImageHash, hex(output.block_hash()));
        EXPECT_EQ(ExpectedOutJson, output.json());
    }

    { // Double check: check if simple signature process gives the same result. Note that private
        // keys were not used anywhere up to this point.
        TW::Nano::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(localInputData.data(), (int)localInputData.size()));
        signingInput.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

        TW::Nano::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        EXPECT_EQ(ExpectedOutJson, output.json());
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, localInputData, {signature, signature}, {publicKey.bytes});
        TW::Nano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(0ul, output.json().size());
        EXPECT_EQ(output.error(), Common::Proto::Error_no_support_n2n);
    }

    { // Negative: empty signatures
        outputData = TransactionCompiler::compileWithSignatures(
            coin, localInputData, {}, {});
        TW::Nano::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(0ul, output.json().size());
        EXPECT_EQ(Common::Proto::Error_invalid_params, output.error());
    }
}

} // namespace TW::Nano::tests
