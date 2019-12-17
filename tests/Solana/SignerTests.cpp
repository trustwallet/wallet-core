// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PublicKey.h"
#include "Solana/Signer.h"
#include "Solana/Transaction.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

TEST(SolanaSigner, SingleSignTransaction) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("7v91N7iZ9mNicL8WfG6cgSCKyRXydQjLh6UYBWwm6y1Q"));

    const auto from = Address(publicKey);
    auto to = Address("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "657ZT4gMXRVQZz1mqzcWbP6sDx6ziM41HMpZRMrZYYP2CaEU3QMj8jxV6xRwM3rUKiXnihXLsXWEPRyw7nn4Ssst");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedHex =
        "01fda1c8ad8872d94f7eab52f9c38dc77e1061f4897e3de2b8469eb0992269f6fa1f173e93dbb2da738ab4e895"
        "9ffa50cd087cdfa889f3a1b8acdd62552f7c1d070100020366c2f508c9c555cacc9fb26d88e88dd54e210bb5a8"
        "bce5687f60d7e75c4cd07fc68b3c894c782b05a9c27fc6c66eb14d4e7d31de9086ab7d2129bcb0493afa020000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000001020200010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);

    const auto additionalPrivateKey =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    signerKeys.push_back(additionalPrivateKey);
    try {
        Signer::sign(signerKeys, transaction);
        FAIL() << "publicKey not found in message.accountKeys";
    } catch (std::invalid_argument const& err) {
        EXPECT_EQ(err.what(), std::string("publicKey not found in message.accountKeys"));
    }
}

TEST(SolanaSigner, MultipleSignTransaction) {
    const auto privateKey0 =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    const auto publicKey0 = privateKey0.getPublicKey(TWPublicKeyTypeED25519);
    const auto address0 = Address(publicKey0);
    ASSERT_EQ(Data(publicKey0.bytes.begin(), publicKey0.bytes.end()),
              Base58::bitcoin.decode("GymAh18wHuFTytfSJWi8eYTA9x5S3sNb9CJSGBWoPRE3"));
    const auto privateKey1 =
        PrivateKey(Base58::bitcoin.decode("GvGmNPMQLZE2VNx3KG2GdiC4ndS8uCqd7PjioPgm9Qhi"));
    const auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeED25519);
    const auto address1 = Address(publicKey1);
    ASSERT_EQ(Data(publicKey1.bytes.begin(), publicKey1.bytes.end()),
              Base58::bitcoin.decode("2oKoYSAHgveX91917v4DUEuN8BNKXDg8KJWpaGyEay9V"));

    Data accountIndexes = {0, 1};
    Data data = {0, 0, 0, 0};
    CompiledInstruction instruction(2, accountIndexes, data);
    std::vector<CompiledInstruction> instructions = {instruction};

    MessageHeader header = {2, 0, 1};
    Address programId("11111111111111111111111111111111");
    std::vector<Address> accountKeys = {address0, address1, programId};
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    Message message(header, accountKeys, recentBlockhash, instructions);

    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    // Sign order should not matter
    signerKeys.push_back(privateKey1);
    signerKeys.push_back(privateKey0);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature0(
        "37beWPhNMfWUz75Tb24TX3PCS89FZscbCgwwLpFnzVfZYPqDpAWruvqzc9eeQYft35H23Vm9Tv1dPwEKWT3vAVPb");
    expectedSignatures.push_back(expectedSignature0);
    Signature expectedSignature1(
        "5NxQshVaAXtQ8YVdcBtCanT62KbxnRfhubjGndFvetgn9AiaoLVZvRGutR5D7FJebRxq8bd6nQXn59LFzavEUrdQ");
    expectedSignatures.push_back(expectedSignature1);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedHex =
        "0269b92a9c9200611a8860844c834ba572dc1b867593c9ab15316e517b9834e0046fab1668b1bb9bf34d5a948d"
        "ca4c46923690ffad3fe8e877ee9c920a0adbbf06db00609388620b50a174c518e240f5de00ac5f019c834aa8bb"
        "10d51b73c723e496e4dee8218370a46dde5f834816e3848e32f20baa9b521cfe6ced01f962230b02000103ed69"
        "c4e13965c8d4df50c63287b392072333fb4fe2eead3ff926d07ec80a44c41ab9aa05053f7941592cd027d40810"
        "96f820b28012aa07d2274b3dfd1cdec30400000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000010202000104000000"
        "00";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}

TEST(SolanaSigner, SignUpdateBlockhash) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("G4VSzrknPBWZ1z2YwUnWTxD1td7wmqR5jMPEJRN6wm8S"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("41a5jYky56M6EWDsFfLaZRxoRtgAJSRWxJnxaJNJELn5"));

    const auto from = Address(publicKey);
    auto to = Address("4iSnyfDKaejniaPc2pBBckwQqV3mDS93go15NdxWJq2y");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Signer::sign(signerKeys, transaction);

    Solana::Hash newBlockhash("GgBaCs3NCBuZN12kCJgAW63ydqohFkHEdfdEXBPzLHq");
    Signer::signUpdateBlockhash(signerKeys, transaction, newBlockhash);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "5aYWTCsc9UZibHcGLxA7SMJ6gJ4KNJovnR8VWTf4m35nHb28wh5LYdN9B48mC1FXVe8zSj3GBNyJyLPLTzWuRYnP");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedHex =
        "01e4fe918e5b3fdb97bd0e02665d8f92465938bdfd4542181366aee0de676e2668184fe3a59d35dbbcdb7fcedd"
        "b7155d740c4917f3eafc5bfd76cb1dd4b9c1d004010002032cb8775e08dca27a9b723611f42d474c5e809df576"
        "e242816cfee4bfc56835da373109b0e40ba3d915ac3ae2417dfd0544369bf31f5ce6e3b8329cdf4e6b058a0000"
        "000000000000000000000000000000000000000000000000000000000000040404040404040404040404040404"
        "040404040404040404040404040404040401020200010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}

TEST(SolanaSigner, SignRawMessage) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("GjXseuD8JavBjKMdd6GEsPYZPV7tMMa46GS2JRS5tHRq"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("3BocAWPm1oNXN5qkAV4QeDUmAPpkTcN1rrmCMWAfsXJY"));

    auto rawMessageData =
        "01000203207be13c43c4528592eaf3fd34e064c641c5be3cb6691877d7ade94dff36734108eaea30723c33b525"
        "07bc54024910612f885e4c80c10b99a047fd42c0acbace00000000000000000000000000000000000000000000"
        "000000000000000000000404040404040404040404040404040404040404040404040404040404040404010202"
        "00010c020000002a00000000000000";

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Data rawTransaction = Signer::signRawMessage(signerKeys, parse_hex(rawMessageData));

    auto expectedHex =
        "016e7f8349977b482bccf0bfc202ad917295803831e59ccb865b97d657464791ebfe3336879b84b9f165e464a3"
        "4751fe30d54b01f3c9f33f969aafe1e85951b10901000203207be13c43c4528592eaf3fd34e064c641c5be3cb6"
        "691877d7ade94dff36734108eaea30723c33b52507bc54024910612f885e4c80c10b99a047fd42c0acbace0000"
        "000000000000000000000000000000000000000000000000000000000000040404040404040404040404040404"
        "040404040404040404040404040404040401020200010c020000002a00000000000000";
    ASSERT_EQ(hex(rawTransaction), expectedHex);
}

TEST(SolanaSigner, SignDelegateStake) {
    const auto privateKeySigner =
        PrivateKey(Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746"));
    const auto publicKeySigner = privateKeySigner.getPublicKey(TWPublicKeyTypeED25519);
    auto signer = Address(publicKeySigner);
    ASSERT_EQ(Data(publicKeySigner.bytes.begin(), publicKeySigner.bytes.end()),
              Base58::bitcoin.decode("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu"));

    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address(STAKE_ADDRESS);
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = addressFromValidatorSeed(signer, voteAddress, programId);

    auto message = Message(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKeySigner);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature(
        "4oUMPVJ6NHGX1DFaxnDKSyZkxuN4osjqZE4mzA4iiNHd9kub3RtVKi3SUvRPsB5x2kB6EuRmBcWR4nbiKeoKF9os");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedHex =
        "01be20567f2db0b09802bf70bbaf397d819c29496c6472a70e2db5c92dff1b666f5f5a4787b07d951bf46435c8"
        "4248a974bb417c789041fbc512d0a06869fb1d0e010006080eba44e56f060007284dc037275a15094c1d6c0697"
        "ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7"
        "d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a9"
        "3ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b21"
        "0000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000"
        "000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78"
        "722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602"
        "00015c030000002000000000000000346a707754717431715a6f52377536753633397a32416e675946474e336e"
        "616b2a00000000000000d00700000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49d"
        "c000000000070201026c000000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4ba"
        "b80eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000070501030405000402000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}
