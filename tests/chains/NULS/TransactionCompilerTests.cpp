// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"
#include "uint256.h"

#include "proto/NULS.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::NULS::tests {

TEST(NULSCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ");
    auto to = std::string("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    uint32_t chainId = 1;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)10000000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)100000000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce);
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1569228280);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(hex(preImage),
              "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00"
              "000000000000000000000000000000000000000000000000000000000800000000000000000001170100"
              "01f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000"
              "0000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "8746b37cb4b443424d3093e8107c5dfd6c5318010bbffcc8e8ba7c1da60877fd");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee045");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f92961ac01d401a6f8"
                  "49acc958c6c9653f49282f5a0916df036ea8766918bac19500");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "0200f885885d00008c01170100012c177a01a7afbe98e094007b99476534fb7926b701000100201d9a00000000"
        "00000000000000000000000000000000000000000000000000080000000000000000000117010001f05e787897"
        "1f3374515eabb6f16d75219d887312010001008096980000000000000000000000000000000000000000000000"
        "00000000000000000000000000006a21033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0a"
        "ff0ee045473045022100a5234269eab6fe8a1510dd0cb36070a03464b48856e1ef2681dbb79a5ec656f9022029"
        "61ac01d401a6f849acc958c6c9653f49282f5a0916df036ea8766918bac195");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    EXPECT_EQ(outputData.size(), 259ul);
    {
        TW::NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 256ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }
}

TEST(NULSCompiler, TokenCompileWithSignatures) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgWabfcG6H7NDK2TJvtoU3wxY1YLKwJ");
    auto to = std::string("NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe");
    uint32_t chainId = 9;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)10000000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)100000000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto feePayerBalance = TW::store((TW::uint256_t)100000000);
    auto feePayerBalanceStr = std::string(feePayerBalance.begin(), feePayerBalance.end());
    auto nonce = std::string("0000000000000000");
    auto asset_nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(asset_nonce.data(), asset_nonce.size());
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1569228280);
    input.set_fee_payer(from);
    input.set_fee_payer_balance(feePayerBalanceStr);
    input.set_fee_payer_nonce(nonce.data(), nonce.size());
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImage),
        "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b70900010080969800000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7af"
        "be98e094007b99476534fb7926b701000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080"
        "969800000000000000000000000000000000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "9040642ce845b320453b2ccd6f80efc38fdf61ec8f0c12e0c16f6244ec2e0496");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("033c87a3d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee045");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const auto signature =
        parse_hex("5ddea604c6cdfcf6cbe32f5873937641676ee5f9aee3c40aa9857c59aefedff25b77429cf62307d4"
                  "3a6a79b4c106123e6232e3981032573770fe2726bf9fc07c00");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "0200f885885d0000d202170100012c177a01a7afbe98e094007b99476534fb7926b70900010080969800000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100012c177a01a7af"
        "be98e094007b99476534fb7926b701000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001f05e7878971f3374515eabb6f16d75219d8873120900010080"
        "9698000000000000000000000000000000000000000000000000000000000000000000000000006921033c87a3"
        "d9b812556b3034b6471cad5131a01e210c1d7ca06dd53b7d0aff0ee04546304402205ddea604c6cdfcf6cbe32f"
        "5873937641676ee5f9aee3c40aa9857c59aefedff202205b77429cf62307d43a6a79b4c106123e6232e3981032"
        "573770fe2726bf9fc07c");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    EXPECT_EQ(outputData.size(), 328ul);

    {
        TW::NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 325ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

// Tx:
// https://nulscan.io/transaction/info?hash=58694394d2e28e18f66cf61697c791774c44d5275dce60d5e05d03df6ede0e22
TEST(NULSCompiler, CompileWithSignaturesFeePayer) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac");
    auto to = std::string("NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV");
    uint32_t chainId = 1;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)100000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)1000000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto feePayerBalance = TW::store((TW::uint256_t)1000000);
    auto feePayerBalanceStr = std::string(feePayerBalance.begin(), feePayerBalance.end());
    auto nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce);
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1660632991);
    // fee payer
    input.set_fee_payer("NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA");
    input.set_fee_payer_nonce("0000000000000000");
    input.set_fee_payer_balance(feePayerBalanceStr);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImage),
        "02009f3ffb620000d202170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a0860100000000"
        "00000000000000000000000000000000000000000000000000080000000000000000001701000152a6388c8bf5"
        "4e8fcd73cc824813bfef0912299b01000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b701000100a0"
        "860100000000000000000000000000000000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "58694394d2e28e18f66cf61697c791774c44d5275dce60d5e05d03df6ede0e22");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("02764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const Data feePayerPublicKeyData =
        parse_hex("025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff");
    const PublicKey feePayerPublicKey = PublicKey(feePayerPublicKeyData, TWPublicKeyTypeSECP256k1);

    const auto signature =
        parse_hex("40b5820b93fd5e272f2a00518af45a722571834934ba20d9a866de8e6d6409ab03c610c647648444"
                  "c1e2193634b2c51817a5a6ac3fe781da1a9ea773506afd8201");
    const auto feePayerSignature =
        parse_hex("140e46b260942a8475f38df39bf54a2eea56c77199fc7ba775aa4b7f147d0d216c82705cba509f37"
                  "ba0e35520a97bccb71a9e35cadcb8d95dd7fde5c8aa9e42800");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    EXPECT_TRUE(feePayerPublicKey.verify(feePayerSignature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    Data outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature, feePayerSignature}, {publicKeyData, feePayerPublicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "02009f3ffb620000d202170100014f019a4227bff89d51590fbf53fbd98d994485f801000100a0860100000000"
        "00000000000000000000000000000000000000000000000000080000000000000000001701000152a6388c8bf5"
        "4e8fcd73cc824813bfef0912299b01000100a08601000000000000000000000000000000000000000000000000"
        "0000000000080000000000000000000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b701000100a0"
        "860100000000000000000000000000000000000000000000000000000000000000000000000000d22102764370"
        "693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022040b5820b93fd5e272f2a00"
        "518af45a722571834934ba20d9a866de8e6d6409ab022003c610c647648444c1e2193634b2c51817a5a6ac3fe7"
        "81da1a9ea773506afd8221025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff46"
        "30440220140e46b260942a8475f38df39bf54a2eea56c77199fc7ba775aa4b7f147d0d2102206c82705cba509f"
        "37ba0e35520a97bccb71a9e35cadcb8d95dd7fde5c8aa9e428");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    EXPECT_EQ(outputData.size(), 433ul);
    TW::NULS::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    EXPECT_EQ(signingOutput.encoded(), ExpectedTx);
    EXPECT_EQ(signingOutput.encoded().size(), 430ul);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("0x48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002");
        signingInput.set_private_key(key.data(), key.size());
        auto feePayerPrivateKey =
            parse_hex("0x9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6");
        signingInput.set_fee_payer_private_key(feePayerPrivateKey.data(), feePayerPrivateKey.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }
}

// Tx:
// https://nulscan.io/transaction/info?hash=210731f7b4a96884a2e04925e758586fb2edb07627621ba1be4de2c54b0868fc
TEST(NULSCompiler, TokenCompileWithSignaturesFeePayer) {
    const auto coin = TWCoinTypeNULS;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::NULS::Proto::SigningInput();
    auto from = std::string("NULSd6HgYx7bdWWv7PxYhYeTRBhD6kZs1o5Ac");
    auto to = std::string("NULSd6HgaHjzhMEUjd4T5DFnLz9EvV4TntrdV");
    uint32_t chainId = 9;
    uint32_t idassetsId = 1;
    auto amount = TW::store((TW::uint256_t)100000);
    auto amountStr = std::string(amount.begin(), amount.end());
    auto balance = TW::store((TW::uint256_t)900000);
    auto balanceStr = std::string(balance.begin(), balance.end());
    auto feePayerBalance = TW::store((TW::uint256_t)1000000);
    auto feePayerBalanceStr = std::string(feePayerBalance.begin(), feePayerBalance.end());
    auto nonce = std::string("0000000000000000");
    input.set_from(from);
    input.set_to(to);
    input.set_amount(amountStr);
    input.set_chain_id(chainId);
    input.set_idassets_id(idassetsId);
    input.set_nonce(nonce);
    input.set_balance(balanceStr);
    input.set_timestamp((uint32_t)1660636748);
    // fee payer
    input.set_fee_payer("NULSd6HgYj81NrQBFZYXvyQhHCJCkGYWDTNeA");
    input.set_fee_payer_nonce("e05d03df6ede0e22");
    input.set_fee_payer_balance(feePayerBalanceStr);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::TxCompiler::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    auto preImage = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImage),
        "02004c4efb620000d2021701000152a6388c8bf54e8fcd73cc824813bfef0912299b09000100a0860100000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100014f019a4227bf"
        "f89d51590fbf53fbd98d994485f801000100a08601000000000000000000000000000000000000000000000000"
        "000000000008e05d03df6ede0e22000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b709000100a0"
        "860100000000000000000000000000000000000000000000000000000000000000000000000000");
    auto preImageHash = preSigningOutput.data_hash();
    EXPECT_EQ(hex(preImageHash),
              "210731f7b4a96884a2e04925e758586fb2edb07627621ba1be4de2c54b0868fc");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("02764370693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeSECP256k1);
    const Data feePayerPublicKeyData =
        parse_hex("025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff");
    const PublicKey feePayerPublicKey = PublicKey(feePayerPublicKeyData, TWPublicKeyTypeSECP256k1);

    const auto signature =
        parse_hex("25cb5b40bda4e6fc0ba719bb0c1a907b2a0faa91316ef2c836310d49f906b6a87d530a56a6c56d97"
                  "4036c86125da06d6e47f9d8ba1544ac3e620cebd883a707800");
    const auto feePayerSignature =
        parse_hex("ff6f45a1c3856f9ea954baca6b2988295bbb22c958f87f0d3baf998993054953426ecb1520513710"
                  "b99ab50e1f6c7e21b0175adef08aa05070bb9bfca8a001d801");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    EXPECT_TRUE(feePayerPublicKey.verify(feePayerSignature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    Data outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature, feePayerSignature}, {publicKeyData, feePayerPublicKeyData});
    const auto ExpectedEncoded = parse_hex(
        "02004c4efb620000d2021701000152a6388c8bf54e8fcd73cc824813bfef0912299b09000100a0860100000000"
        "0000000000000000000000000000000000000000000000000008000000000000000000170100014f019a4227bf"
        "f89d51590fbf53fbd98d994485f801000100a08601000000000000000000000000000000000000000000000000"
        "000000000008e05d03df6ede0e22000117010001686a3c9cd2ac45aee0ef825b0443d1eb209368b709000100a0"
        "860100000000000000000000000000000000000000000000000000000000000000000000000000d32102764370"
        "693450d654d6fc061d1d4dbfbe0c95715fd3cde7e15df073ab0983b8c8463044022025cb5b40bda4e6fc0ba719"
        "bb0c1a907b2a0faa91316ef2c836310d49f906b6a802207d530a56a6c56d974036c86125da06d6e47f9d8ba154"
        "4ac3e620cebd883a707821025910df09ca768909cce9224d182401044c7b5bd44b0adee2ec5f2e64446573ff47"
        "3045022100ff6f45a1c3856f9ea954baca6b2988295bbb22c958f87f0d3baf9989930549530220426ecb152051"
        "3710b99ab50e1f6c7e21b0175adef08aa05070bb9bfca8a001d8");
    const auto ExpectedTx = std::string(ExpectedEncoded.begin(), ExpectedEncoded.end());
    EXPECT_EQ(outputData.size(), 434ul);
    TW::NULS::Proto::SigningOutput signingOutput;
    ASSERT_TRUE(signingOutput.ParseFromArray(outputData.data(), (int)outputData.size()));
    EXPECT_EQ(signingOutput.encoded(), ExpectedTx);
    EXPECT_EQ(signingOutput.encoded().size(), 431ul);

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        TW::NULS::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("0x48c91cd24a27a1cdc791022ff39316444229db1c466b3b1841b40c919dee3002");
        signingInput.set_private_key(key.data(), key.size());
        auto feePayerPrivateKey =
            parse_hex("0x9401fd554cb700777e57b05338f9ff47597add8b23ce9f1c8e041e9b4e2116b6");
        signingInput.set_fee_payer_private_key(feePayerPrivateKey.data(), feePayerPrivateKey.size());

        TW::NULS::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }

    { // Negative: inconsistent signatures & publicKeys
        outputData = TransactionCompiler::compileWithSignatures(
            coin, inputStrData, {signature, signature}, {publicKeyData});
        NULS::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));
        EXPECT_EQ(output.encoded().size(), 0ul);
        EXPECT_EQ(output.error(), Common::Proto::Error_signatures_count);
    }
}

} // namespace TW::NULS::tests
