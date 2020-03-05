// Copyright © 2017-2020 Trust Wallet.
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
        "5T6uZBHnHFd8uWErDBTFRVkbKuhbcm94K5MJ2beTYDruzqv4FjS7EMKvC94ZfxNAiWUXZ6bZxS3WXUbhJwYNPWn");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
        "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
        "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
        "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";
    ASSERT_EQ(transaction.serialize(), expectedString);

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

    auto expectedString =
        "oL2CmkcP9xf2DiU7eo6hh3JdHnX3NGjunheXYo6SjVchzc8LtFJpPs4jccWUd7oPZUPQNTcR7Ee"
        "Hn259ror9A7aXgJdP4djhntoD8irF1kuBZCj7pubtoWfiAKzagSL4hChQsTSe7e9jaGtoXu58mP"
        "HCMKTz55TLjhdmCj7ixoWRowWEzkrF49MxXnurb4yf6ASru1XdHPFn3DdzkRHgypYwvRM6ci8p2"
        "7trQvXFukhWX6qG6JkxqsWYSzACcAAGGWfAxSi63Yx1RxkxGUzyxy5f2thQhWZ6Nx6pR1im65yV"
        "YMYPXj94kgtHxXw9h5V4p7xSAwRpmhw4jewYyQVX4jmnfro3gFNdX9AqpqMs4uGHA4rZM";
    ASSERT_EQ(transaction.serialize(), expectedString);
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
        "5AFhXjvGdENXCAe9MPvUA2qjoL4XtZwZKG7kK2HmZf1ibpxjx5kzogHZjN39uYB9J33UFJN15KhSggBZhzyNQmta");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "62ABadDCoPfGGRnhLoBhfcPekMHyN5ee8DgTY8wD4iwKDjyFAsNbsaahTcqMWxmwa61q9iAGCQB"
        "v1bETcYzWsTwLKMVGLoEpwqA84mPjqHyr5sQD5dcghyQiQ1ckYNub9K7s8FspVwwowK8gJG69xe"
        "DEaqi7G1zrChBVbQYTmVUwJETyDmP1Vs8QU3CaxBs8qwcxoziU52KWLBpRj9o38QVBdxJtJ7hig"
        "hgPKJubfqUfTWdN94PzqEfyPqwoCpFD39nvBn8C5xe1caPKivicg6U7Lzm9s8RYTLCEB";
    ASSERT_EQ(transaction.serialize(), expectedString);
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
        "4Q3ctvLybffZJBGdvrnEg171AjZeiMPGf3rVfGCpDVZRq9QaKVw8jWTMqHzZckGi2yXbGoQdBgeua24iMfPGHTtV");
    expectedSignatures.push_back(expectedSignature);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    auto expectedString =
        "7gcsGMV8fPkuSDegrBk5KF4eZtmGqdixEr5Kxx7m74nErH5EWxkKcvy5LKg3kqKHLMtzqGXrM8EdVskLu5k6cQUVmz"
        "nfd6iGpuDkn4BWi8HaqSoaeL7DCx6QHLUwcyvorfqPkgSaYopY9MeB6MGKS4bKCQi1paP7eEKZj9JgAZTHnpzAuFzq"
        "E5dC98MrbThoBmHqfybw1jHssp4NZjrxsuMz4oYcHe837WmnpinE68QEVf9FzuoXKrMDwYZQYogJHUZgxghvMmJu3X"
        "VZk7hL1h7SgE9ow2SvvNfAUYfUTuz8N9m6JcXgwmNL51ZweW9F1fekmSKE3vLEAaVvvYFsDxn1gt4bXuTdAMuUDKdH"
        "33YMLQr8eEsdUDZUm33KtuE7Ddy84NEg3KbpWAy8T4vkwiLyvoyjmMdzCkMuobyXWQV1rfRk5vTJ4x6dMgznfAeKEq"
        "md6xZ2hN8JBmTenP5dsvZCCSCf4G7cUkdndKu552KALqiNHd5msgLJvJKSHmZjFhS43fDCvkG7njF8yaZzWjmAknWU"
        "BbV6YaGmD3XmcWfJgvB1zivZJhiMbzopP8Nm5wL5iDbCrSZTGq2tzEsTvje75wv2RtuAcgiicEPTuPAin9fKyMbCpf"
        "67pGgWPwH5DwYumMwd8zwoJyuakyqFsFLnBKTvp8pFCijdj7fEhyC31xuMV7crwyrN5X3y7QKCE7PZcBP637YHEPtT"
        "vt1ECp4CqBSnvPc8vRD8EMhHe5jRFSDkQriUenEPFc51dTDTJWL26xuiTivktEm6ahHq5d6MPr4NRDvcRG2cZvEgxH"
        "BLpKfuB5XL3JfQZ3Nn3B916gaK8owz9Rk2e3";
    ASSERT_EQ(transaction.serialize(), expectedString);
}
