// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionCompiler.h"

#include "proto/Solana.pb.h"

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCoinType.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Solana::tests {

TEST(SolanaCompiler, CompileTransferWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_transfer_transaction();
    auto recipient = std::string("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("TPJFTN4CjBn12HiBfAbGUhpD9zGvRSm2RcheFRA4Fyv"));
    message.set_recipient(recipient);
    message.set_value((uint64_t)1000);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(hex(preImageHash),
              "010001030d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c024c255a8bc3e"
              "8496217a2cd2a1894b9b9dcace04fcd9c0d599acdaaea40a1b6100000000000000000000000000000000"
              "0000000000000000000000000000000006c25012cc11a599a45b3b2f7f8a7c65b0547fa0bb67170d7a0c"
              "d1eda4e2c9e501020200010c02000000e803000000000000");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData =
        parse_hex("0d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0");
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature =
        parse_hex("a8c610697087eaf8a34b3facbe06f8e9bb9603bb03270dad021ffcd2fc37b6e9efcdcb78b227401f"
                  "000eb9231c67685240890962e44a17fd27fc2ff7b971df03");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "5bWxVCP5fuzkKSGby9hnsLranszQJR2evJGTfBrpDQ4rJceW1WxKNrWqVPBsN2QCAGmE6W7VaYkyWjv39HhGrr1Ne2"
        "QSUuHZdyyn7hK4pxzLPMgPG8fY1XvXdppWMaKMLmhriLkckzGKJMaE3pWBRFBKzigXY28714uUNndb7S9hVakxa59h"
        "rLph39CMgAkcj6b8KYvJEkb1YdYytHSZNGi4kVVTNqiicNgPdf1gmG6qz9zVtnqj9JtaD2efdS8qxsKnvNWSgb8Xxb"
        "T6dwyp7msUUi7d27cYaPTpK";
    EXPECT_EQ(outputData.size(), 296ul);
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 293ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, CompileCreateNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    uint64_t rent = 10000000;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("mFmK2xFMhzJJaUN5cctfdCizE9dtgcSASSEDh1Yzmat"));
    message.set_nonce_account(nonceAccount);
    message.set_rent(rent);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 2);
    auto signer1 = preSigningOutput.signers(0);
    EXPECT_EQ(signer1, sender);
    auto signer2 = preSigningOutput.signers(1);
    EXPECT_EQ(signer2, nonceAccount);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "020003050d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb447a670"
        "0c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b2"
        "1ab344d8062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000000000"
        "00000000000000000000000000000000000000000000000000000000000b563fd13b46e844f12f54fa8a0e78c4"
        "4d95dbae4953368b7135f1e0de111cb50204020001340000000080969800000000005000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000040301020324060000000d044a62d0a4"
        "dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c0");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const Data nonceAccountPublicKeyData = Base58::decode(nonceAccount);
    const PublicKey nonceAccountPublicKey =
        PublicKey(nonceAccountPublicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::decode(
        "3dbiGHLsFqnwA1PXx7xmoikzv6v9g9BXvZts2126qyE163BypurkvgbDiF5RmrEZRiT2MG88v6xwyJTkhhDRuFc9");
    const auto nonceAccountSignature = Base58::decode(
        "jFq4PbbEM1fuPbq5CkUYgzs7a21g6rvFkfLJAUUGP5QMKYhHBE6nB1dqtwaJsABgyUvrR8QjT2Ej73cXNz7Vur1");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    EXPECT_TRUE(nonceAccountPublicKey.verify(nonceAccountSignature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature, nonceAccountSignature},
        {publicKeyData, nonceAccountPublicKeyData});
    const auto ExpectedTx =
        "3wu6xJSbb2NysVgi7pdfMgwVBT1knAdeCr9NR8EktJLoByzM4s9SMto2PPmrnbRqPtHwnpAKxXkC4vqyWY2dRBgdGG"
        "CC1bep6qN5nSLVzpPYAWUSq5cd4gfYMAVriFYRRNHmYUnEq8vMn4vjiECmZoHrpabBj8HpXGqYBo87sbZa8ZPCxUcB"
        "71hxXiHWZHj2rovx2kr75Uuv1buWXyW6M8uR4UNvQcPPvzVbwBG82RjDYTuancMSAxmrVNR8GLBQNhrCCYrZyte3EW"
        "gEyMQxxfW8T3xNXqnbgdfvFJ3UjRBxXj3hrmv17xEivTjfs81aG2AAi24yiYrk8ep7eQqwDHVSArsrynnwVKVNUcCQ"
        "CnSy7fuiuS7FweFX8DEN1K9BrfecHyWrF15fYzhkmWSs64aH6ZTYHWPv5znhFKYmAuopGwbsBEb2j5p8NS3iJZ2skb"
        "2wi47n1rpLZfoCHWKxNiikkDUJTGQNcSDrGUMfeW5aGubJrCfecPKEo9Wo9kd36iSsxYPYSWNKrz2HTooa1rCRhqjX"
        "D8dyX3bXGV8TK6W2sEgf4JkcDnNoWQLbindcP8XR";

    EXPECT_EQ(outputData.size(), 583ul);

    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 580ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        auto nonceAccountKey =
            parse_hex("2a9737aca3cde2dc0b4f3ae3487e3a90000490cb39fbc979da32b974ff5d7490");
        signingInput.set_private_key(key.data(), key.size());
        auto& aMessage = *signingInput.mutable_create_nonce_account();
        aMessage.set_nonce_account_private_key(nonceAccountKey.data(), nonceAccountKey.size());
        aMessage.set_rent(rent);

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, CompileWithdrawNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_withdraw_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    auto sender = std::string("sp6VUqq1nDEuU83bU2hstmEYrJNipJYpwS7gZ7Jv7ZH");
    auto recipient = std::string("3UVYmECPPMZSCqWKfENfuoTv51fTDTWicX9xmBD2euKe");
    uint64_t value = 10000000;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("5ccb7sRth3CP8fghmarFycr6VQX3NcfyDJsMFtmdkdU8"));
    message.set_nonce_account(nonceAccount);
    message.set_recipient(recipient);
    message.set_value(value);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(hex(preImageHash),
              "010003060d044a62d0a4dfe5a037a15b59fa4d4d0d3ab81103a2c10a6da08a4d058611c057f6ed937bb4"
              "47a6700c9684d2e182b1a6661838a86cca7d0aac18be2e098b2124c255a8bc3e8496217a2cd2a1894b9b"
              "9dcace04fcd9c0d599acdaaea40a1b6106a7d517192c568ee08a845f73d29788cf035c3145b21ab344d8"
              "062ea940000006a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a0000000000000000"
              "00000000000000000000000000000000000000000000000000000000448e50d73f42e3163f5926922aad"
              "d2bca6bdd91f97b3eb7b750e2cecfd810f6d01050501020304000c050000008096980000000000");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::decode(
        "MxbTCAUmBLiESDLK1NiK5ab41mL2SpAPKSbvGdYQQD5eKgAJRdFEJ8MV9HqBhDQHdsS2LG3QMQQVJp51ekGu6KM");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    const Data outputData = TransactionCompiler::compileWithSignatures(
        coin, inputStrData, {signature},
        {publicKeyData});
    const auto ExpectedTx =
        "7gdEdDymvtfPfVgVvCTPzafmZc1Z8Zu4uXgJDLm8KGpLyPHysxFGjtFzimZDmGtNhQCh22Ygv3ZtPZmSbANbafikR3"
        "S1tvujatHW9gMo35jveq7TxwcGoNSqc7tnH85hkEZwnDryVaiKRvtCeH3dgFE9YqPHxiBuZT5eChHJvVNb9iTTdMsJ"
        "XMusRtzeRV45CvrLKUvsAH7SSWHYW6bGow5TbEJie4buuz2rnbeVG5cxaZ6vyG2nJWHNuDPWZJTRi1MFEwHoxst3a5"
        "jQPv9UrG9rNZFCw4uZizVcG6HEqHWgQBu8gVpYpzFCX5SrhjGPZpbK3YmHhUEMEpJx3Fn7jX7Kt4t3hhhrieXppoqK"
        "NuqjeNVjfEf3Q8dJRfuVMLdXYbmitCVTPQzYKWBR6ERqWLYoAVqjoAS2pRUw1nrqi1HR";
    EXPECT_EQ(outputData.size(), 431ul);
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 428ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = parse_hex("044014463e2ee3cc9c67a6f191dbac82288eb1d5c1111d21245bdc6a855082a1");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, CompileCreateTokenAccountAndTransferWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = std::string("3WUX9wASxyScbA7brDipioKfXS1XEYkQ4vo3Kej9bKei");
    auto recipientTokenAddress = std::string("BwTAyrCEdkjNyGSGVNSSGh6phn8KQaLN638Evj7PVQfJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D"));
    message.set_recipient_main_address(recipientMainAddress);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "0100060994c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685aa287d36838b4ef65c2"
        "c460d1a52498453c259cd6a35ca91064aaead28187ca69485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551"
        "b5fd4eacd82d65e302202544558bb05c2698f88852a7925c5c0ee5ea8711ddb3fe1262150283eee811633b442c"
        "b3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea700000000000000000000000000000000"
        "0000000000000000000000000000000006ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857e"
        "ff00a906a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000008c97258f4e2489f1bb3d"
        "1029148e0d830b5a1399daff1084048e7bd8dbe9f8598fb6d19edbbae20ebbc767fba1da4d4b40a4b97479fe52"
        "6a82325cba7cee506802080700010304050607000604020401000a0ca00f00000000000006");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::decode(
        "pL1m11UEDWn3jMkrNMqLeGwNpKzmhQzJiYaCocgPy7vXKA1tnvEjJbuVq9hTeM9kqMAmxhRpwRY157jDgkRdUZw");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "2qkvFTcMk9kPaHtd7idJ1gJc4zTkuYDUJsC67kXvHjv3zwEyUx92QyhhSeBjL6h3Zaisj2nvTWid2UD1N9hbg9Ty7v"
        "SHLc7mcFVvy3yJmN9tz99iLKsf15rEeKUk3crXWLtKZEpcXJurN7vrxKwjQJnVob2RjyxwVfho1oNZ72BHvqToRM1W"
        "2KbcYhxK4d9zB4QY5tR2dzgCHWqAjf9Yov3y9mPBYCQBtw2GewrVMDbg5TK81E9BaWer3BcEafc3NCnRfcFEp7ZUXs"
        "GAgJYx32uUoJPP8ByTqBsp2JWgHyZhoz1WUUYRqWKZthzotErVetjSik4h5GcXk9Nb6kzqEf4nKEZ22eyrP5dv3eZM"
        "uGUUpMYUT9uF16T72s4TTwqiWDPFkidD33tACx74JKGoDraHEvEeAPrv6iUmC675kMuAV4EtVspVc5SnKXgRWRxb4d"
        "cH3k7K4ckjSxYZwg8UhTXUgPxA936jBr2HeQuPLmNVn2muA1HfL2DnyrobUP9vHpbL3HHgM2fckeXy8LAcjnoE9TTa"
        "AKX32wo5xoMj9wJmmtcU6YbXN4KgZ";
    EXPECT_EQ(outputData.size(), 572ul);
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 569ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, SolanaCompileAdvanceNonceAccountWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_advance_nonce_account();
    auto nonceAccount = std::string("6vNrYDm6EHcvBALY7HywuDWpTSc6uGt3y2nf5MuG1TmJ");
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("4KQLRUfd7GEVXxAeDqwtuGTdwKd9zMfPycyAG3wJsdck"));
    message.set_nonce_account(nonceAccount);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 1);
    auto signer = preSigningOutput.signers(0);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "0100020494c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a57f6ed937bb447a670"
        "0c9684d2e182b1a6661838a86cca7d0aac18be2e098b2106a7d517192c568ee08a845f73d29788cf035c3145b2"
        "1ab344d8062ea940000000000000000000000000000000000000000000000000000000000000000000003149e6"
        "70959884ea98bb33bca21c9505f1fc17b1d51ca59555a5d58c93f0f9c90103030102000404000000");
    // Simulate signature, normally obtained from signature server
    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature = Base58::decode(
        "2gwuvwJ3mdEsjA8Gid6FXYuSwa2AAyFY6Btw8ifwSc2SPsfKBnD859C5mX4tLy6zQFHhKxSMMsW49o3dbJNiXDMo");
    // Verify signature (pubkey & hash & signature)
    EXPECT_TRUE(publicKey.verify(signature, TW::data(preImageHash)));
    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature}, {publicKeyData});
    const auto ExpectedTx =
        "7YPgNzjCnUd2zBb6ZC6bf1YaoLjhJPHixLUdTjqMjq1YdzADJCx2wsTTBFFrqDKSHXEL6ntRq8NVJTQMGzGH5AQRKw"
        "tKtutehxesmtzkZCPY9ADZ4ijFyveLmTt7kjZXX7ZWVoUmKAqiaYsPTex728uMBSRJpV4zRw2yKGdQRHTKy2QFEb9a"
        "cwLjmrbEgoyzPCarxjPhw21QZnNcy8RiYJB2mzZ9nvhrD5d2jB5TtdiroQPgTSdKFzkNEd7hJUKpqUppjDFcNHGK73"
        "FE9pCP2dKxCLH8Wfaez8bLtopjmWun9cbikxo7LZsarYzMXvxwZmerRd1";
    EXPECT_EQ(outputData.size(), 330ul);
    {

        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 327ul);
    }

    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, CompileCreateTokenAccountAndTransferWithExternalFeePayerWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_create_and_transfer_token_transaction();
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    auto recipientMainAddress = std::string("E54BymfdhXQtzDSGtgiJayauEMdB2gJjPqoDjzfbCXwj");
    auto recipientTokenAddress = std::string("Ay7G7Yb7ZCebCQdG39FxD1nFNDtqFWJCBgfd5Ek7DDcS");
    auto feePayer = std::string("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("EsnN3ksLV6RLBW7qqgrvm2diwVJNTzL9QCuzm6tqWsU8"));
    input.set_fee_payer(feePayer);
    message.set_recipient_main_address(recipientMainAddress);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 2);
    auto feepayerSigner = preSigningOutput.signers(0);
    EXPECT_EQ(feepayerSigner, feePayer);
    auto signer = preSigningOutput.signers(1);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "0200060acb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b5794c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a9418c9576a9c00c6bd8fc223f471573f7172488de10aa84dbf63c53a20bae717485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551b5fd4eacd82d65e30220c231dc02f482980f7d9915c1ecf53374091d38c060b49487f9c5d932e077ed763b442cb3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea7000000000000000000000000000000000000000000000000000000000000000006ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857eff00a906a7d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a000000008c97258f4e2489f1bb3d1029148e0d830b5a1399daff1084048e7bd8dbe9f859ce2a4331bce3670e6ea8bedff5908c6d91f833a31a7fdeac16978c261a1801d502090700020405060708000704030502010a0ca00f00000000000006");
    // Simulate signature, normally obtained from signature server
    // Verify signature (pubkey & hash & signature)
    const Data feePayerPublicKeyData = Base58::decode(feePayer);
    const PublicKey feePayerPublicKey = PublicKey(feePayerPublicKeyData, TWPublicKeyTypeED25519);
    const auto signature1 = Base58::decode(
        "7GZGFT2VA4dpJBBwjiMqj1o8yChDvoCsqnQ7xz4GxY513W3efxRqbB8y7g4tH2GEGQRx4vCkKipG1sMaDEen1A2");
    EXPECT_TRUE(feePayerPublicKey.verify(signature1, TW::data(preImageHash)));

    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature2 = Base58::decode(
        "3n7RHTCBAtnFVuDn5eRbyQB24h6AqajJi5nGMPrfnUVFUDh2Cb8AoaJ7mVtjnv73V4HaJCzSwCLAj3zcGEaFftWZ");
    EXPECT_TRUE(publicKey.verify(signature2, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature1, signature2}, {feePayerPublicKeyData, publicKeyData});
    const auto ExpectedTx =
        "5sxFkQYd2FvqRU64N79A6xjJKNkgUsEEg2wKgai2NiK7A7hF3q5GYEbjQsYBG9S2MejwTENbwHzvypaa3D3cEkxvVTg19aJFWdCtXQiz42QF5fN2MuAb6eJR4KHFnzCtxxnYGtN9swZ5B5cMSPCffCRZeUTe3kooRmbTYPvSaemU6reVSM7X2beoFKPd2svrLFa8XnvhBwL9EiFWQ9WhHB2cDV7KozCnJAW9kdNDR4RbfFQxboANGo3ZGE5ddcZ6YdomATKze1TtHj2qzJEJRwxsRr3iM3iNFb4Eav5Q2n71KUriRf73mo44GQUPbQ2LvpZKf4V6M2PzxJwzBo7FiFZurPmsanT3U5efEsKnnueddbiLHedc8JXc1d3Z53sFxVGJpsGA8RR6thse9wUvaEWqXVtPbNA6NMao9DFGD6Dudza9pJXSobPc7mDHZmVmookf5vi6Lb9Y1Q4EgcEPQmbaDnKGGB6uGfZe629i3iKXRzAd2dB7mKfffhDadZ8S1eYGT3dhddV3ExRxcqDP9BAGQT3rkRw1JpeSSi7ziYMQ3vn4t3okdgQSq6rrpbPDUNG8tLSHFMAq3ydnh4Cb4ECKkYoz9SFAnXACUu4mWETxijuKMK9kHrTqPGk9weHTzobzCC8q8fcPWV3TcyUyMxsbVxh5q1p5h5tWfD9td5TZJ2HEUbTop2dA53ZF";
    EXPECT_EQ(outputData.size(), 703ul);
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 700ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());
        auto feePayerKey = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
        signingInput.set_fee_payer_private_key(feePayerKey.data(), feePayerKey.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

TEST(SolanaCompiler, CompileTokenTransferWithExternalFeePayerWithSignatures) {
    const auto coin = TWCoinTypeSolana;
    /// Step 1: Prepare transaction input (protobuf)
    auto input = TW::Solana::Proto::SigningInput();
    auto& message = *input.mutable_token_transfer_transaction();
    auto sender = std::string("B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V");
    auto token = std::string("4zMMC9srt5Ri5X14GAgXhaHii3GnPAEERYPJgZJDncDU");
    auto senderTokenAddress = std::string("5sS5Z8GAdVHqZKRqEvpDauHvvLgbDveiyfi81uh25mrf");
    // auto recipientMainAddress = std::string("E54BymfdhXQtzDSGtgiJayauEMdB2gJjPqoDjzfbCXwj");
    auto recipientTokenAddress = std::string("AZapcpAZtEL1gQuC87F2L1hSfAZnAvNy1hHtJ8DJzySN");
    auto feePayer = std::string("Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ");
    uint64_t amount = 4000;
    uint8_t decimals = 6;
    input.set_sender(sender);
    input.set_recent_blockhash(std::string("GwB5uixiTQUG2Pvo6fWAaNQmz41Jt4WMEPD7b83wvHkX"));
    input.set_fee_payer(feePayer);
    message.set_token_mint_address(token);
    message.set_recipient_token_address(recipientTokenAddress);
    message.set_sender_token_address(senderTokenAddress);
    message.set_amount(amount);
    message.set_decimals(decimals);
    auto inputString = input.SerializeAsString();
    auto inputStrData = TW::Data(inputString.begin(), inputString.end());
    /// Step 2: Obtain preimage hash
    const auto preImageHashesData = TransactionCompiler::preImageHashes(coin, inputStrData);
    auto preSigningOutput = TW::Solana::Proto::PreSigningOutput();
    preSigningOutput.ParseFromArray(preImageHashesData.data(), (int)preImageHashesData.size());
    ASSERT_EQ(preSigningOutput.signers_size(), 2);
    auto feepayerSigner = preSigningOutput.signers(0);
    EXPECT_EQ(feepayerSigner, feePayer);
    auto signer = preSigningOutput.signers(1);
    EXPECT_EQ(signer, sender);
    auto preImageHash = preSigningOutput.data();
    EXPECT_EQ(
        hex(preImageHash),
        "02000206cb2af089b56a557737bc1718e0cbf232cf5b02e14ee0aa7c6675233f5f6f9b5794c3890fa8d4bc04ab2a676d2cafea5cdc899ecd95a9cbe593e9df258759685a485a24ffb4070461bb6d7f1c8b758c6b2dc90029d551b5fd4eacd82d65e302208e12027e9261a6a276b5ff00ddecfda567ff3ae510a5b47045086ad1d50cab573b442cb3912157f13a933d0134282d032b5ffecd01a2dbf1b7790608df002ea706ddf6e1d765a193d9cbe146ceeb79ac1cb485ed5f5b37913a8cf5857eff00a9ecc01200a43c87ad04ab8b382c0934a54e585e7dcd9cdef6d1cacd52718981c4010504020403010a0ca00f00000000000006");
    // Simulate signature, normally obtained from signature server
    // Verify signature (pubkey & hash & signature)
    const Data feePayerPublicKeyData = Base58::decode(feePayer);
    const PublicKey feePayerPublicKey = PublicKey(feePayerPublicKeyData, TWPublicKeyTypeED25519);
    const auto signature1 = Base58::decode(
        "2LbovMDuKoR2LFcV5NbK9bCQZcTG99W6VE1urvdWFWvRhNg9ocDGhayyeBGisoqZgYZtcD3b6LJDTmPx9Gp3T6qd");
    EXPECT_TRUE(feePayerPublicKey.verify(signature1, TW::data(preImageHash)));

    const Data publicKeyData = Base58::decode(sender);
    const PublicKey publicKey = PublicKey(publicKeyData, TWPublicKeyTypeED25519);
    const auto signature2 = Base58::decode(
        "2hUHMS9rwbUbXrpC7sK7utL2M4soTyQ7EX3sBYvdee9wraJvYoPH2XjovHDn8eRFY8z5uCx9DCj2Zfjpmzfa81Db");
    EXPECT_TRUE(publicKey.verify(signature2, TW::data(preImageHash)));

    /// Step 3: Compile transaction info
    auto outputData =
        TransactionCompiler::compileWithSignatures(coin, inputStrData, {signature1, signature2}, {feePayerPublicKeyData, publicKeyData});
    const auto ExpectedTx =
        "qjgNVBmoPDHNTN2ENQfxNVE57jWXpqdmu5GQX4msA7iK8ZRAnKpvbusQagv8CZGyNYti23p9jBsjTSx75ZU26UW5vgC8D88pusW8W5dp1ERo5DSfurMSYJ6afgQHdcuzn7exb8znSm6uV4y1cWgBRcuAGdg3wRpVhP8HEB1EeKgzjYVWvMSy6yR7qVrSL6BxHG6eiAMyahLFbEt4qBqLEdxxY7Dt4DyydVYmG2ZVtheaMHD3ACwCjpyPLXj399wxSgGXQQFGtzEJQw9awVezmJ4wZk6W4dDpXQvdKYaqUvwTwRZsQB5o2iekPWZXR9xvHiMLjMVBPzYgcU14ZSaCbqSNVv2pAJxP1sMvxZMNMzZPttPxCsDDGq9biC7exXwzesXSnZ3rsgEYeZtkUiBHAxR4rYqBpA6VzLs1bPx8MPTvr9mhNi2ezMBbg2nEfHV6Fz7H7rEY2g3jDtRz35Vmgits8s9RKi3kb73WtGUieRiXjiqkNhpvKkST1oEYRQ9";
    EXPECT_EQ(outputData.size(), 514ul);
    {
        Solana::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(outputData.data(), (int)outputData.size()));

        EXPECT_EQ(output.encoded(), ExpectedTx);
        EXPECT_EQ(output.encoded().size(), 511ul);
    }
    { // Double check: check if simple signature process gives the same result. Note that private
      // keys were not used anywhere up to this point.
        Solana::Proto::SigningInput signingInput;
        ASSERT_TRUE(signingInput.ParseFromArray(inputStrData.data(), (int)inputStrData.size()));
        auto key = Base58::decode("9YtuoD4sH4h88CVM8DSnkfoAaLY7YeGC2TarDJ8eyMS5");
        signingInput.set_private_key(key.data(), key.size());
        auto feePayerKey = Base58::decode("66ApBuKpo2uSzpjGBraHq7HP8UZMUJzp3um8FdEjkC9c");
        signingInput.set_fee_payer_private_key(feePayerKey.data(), feePayerKey.size());

        Solana::Proto::SigningOutput output;
        ANY_SIGN(signingInput, coin);

        ASSERT_EQ(output.encoded(), ExpectedTx);
    }
}

} // namespace TW::Solana::tests
