// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    Data expectedTransactionBytes = {
        1,   253, 161, 200, 173, 136, 114, 217, 79,  126, 171, 82,  249, 195, 141, 199, 126, 16,
        97,  244, 137, 126, 61,  226, 184, 70,  158, 176, 153, 34,  105, 246, 250, 31,  23,  62,
        147, 219, 178, 218, 115, 138, 180, 232, 149, 159, 250, 80,  205, 8,   124, 223, 168, 137,
        243, 161, 184, 172, 221, 98,  85,  47,  124, 29,  7,   1,   0,   2,   3,   102, 194, 245,
        8,   201, 197, 85,  202, 204, 159, 178, 109, 136, 232, 141, 213, 78,  33,  11,  181, 168,
        188, 229, 104, 127, 96,  215, 231, 92,  76,  208, 127, 198, 139, 60,  137, 76,  120, 43,
        5,   169, 194, 127, 198, 198, 110, 177, 77,  78,  125, 49,  222, 144, 134, 171, 125, 33,
        41,  188, 176, 73,  58,  250, 2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
        2,   2,   0,   1,   12,  2,   0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};
    ASSERT_EQ(transaction.serialize(), expectedTransactionBytes);

    const auto additionalPrivateKey =
        PrivateKey(Base58::bitcoin.decode("96PKHuMPtniu1T74RvUNkbDPXPPRZ8Mg1zXwciCAyaDq"));
    signerKeys.push_back(additionalPrivateKey);
    try {
        Signer::sign(signerKeys, transaction);
        FAIL() << "publicKey not found in message.accountKeys";
    } catch (std::invalid_argument const &err) {
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

    Data expectedTransactionBytes = {
        2,   105, 185, 42,  156, 146, 0,   97,  26,  136, 96,  132, 76,  131, 75,  165, 114,
        220, 27,  134, 117, 147, 201, 171, 21,  49,  110, 81,  123, 152, 52,  224, 4,   111,
        171, 22,  104, 177, 187, 155, 243, 77,  90,  148, 141, 202, 76,  70,  146, 54,  144,
        255, 173, 63,  232, 232, 119, 238, 156, 146, 10,  10,  219, 191, 6,   219, 0,   96,
        147, 136, 98,  11,  80,  161, 116, 197, 24,  226, 64,  245, 222, 0,   172, 95,  1,
        156, 131, 74,  168, 187, 16,  213, 27,  115, 199, 35,  228, 150, 228, 222, 232, 33,
        131, 112, 164, 109, 222, 95,  131, 72,  22,  227, 132, 142, 50,  242, 11,  170, 155,
        82,  28,  254, 108, 237, 1,   249, 98,  35,  11,  2,   0,   1,   3,   237, 105, 196,
        225, 57,  101, 200, 212, 223, 80,  198, 50,  135, 179, 146, 7,   35,  51,  251, 79,
        226, 238, 173, 63,  249, 38,  208, 126, 200, 10,  68,  196, 26,  185, 170, 5,   5,
        63,  121, 65,  89,  44,  208, 39,  212, 8,   16,  150, 248, 32,  178, 128, 18,  170,
        7,   210, 39,  75,  61,  253, 28,  222, 195, 4,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   1,   2,   2,   0,   1,   4,   0,   0,   0,   0};
    ASSERT_EQ(transaction.serialize(), expectedTransactionBytes);
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

    Data expectedTransactionBytes = {
        1,   228, 254, 145, 142, 91,  63,  219, 151, 189, 14,  2,   102, 93, 143, 146, 70,  89,
        56,  189, 253, 69,  66,  24,  19,  102, 174, 224, 222, 103, 110, 38, 104, 24,  79,  227,
        165, 157, 53,  219, 188, 219, 127, 206, 221, 183, 21,  93,  116, 12, 73,  23,  243, 234,
        252, 91,  253, 118, 203, 29,  212, 185, 193, 208, 4,   1,   0,   2,  3,   44,  184, 119,
        94,  8,   220, 162, 122, 155, 114, 54,  17,  244, 45,  71,  76,  94, 128, 157, 245, 118,
        226, 66,  129, 108, 254, 228, 191, 197, 104, 53,  218, 55,  49,  9,  176, 228, 11,  163,
        217, 21,  172, 58,  226, 65,  125, 253, 5,   68,  54,  155, 243, 31, 92,  230, 227, 184,
        50,  156, 223, 78,  107, 5,   138, 0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,
        0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4,   4,   4,   4,
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4,   4,   4,   1,
        2,   2,   0,   1,   12,  2,   0,   0,   0,   42,  0,   0,   0,   0,  0,   0,   0};
    ASSERT_EQ(transaction.serialize(), expectedTransactionBytes);
}

TEST(SolanaSigner, SignRawMessage) {
    const auto privateKey =
        PrivateKey(Base58::bitcoin.decode("GjXseuD8JavBjKMdd6GEsPYZPV7tMMa46GS2JRS5tHRq"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(Data(publicKey.bytes.begin(), publicKey.bytes.end()),
              Base58::bitcoin.decode("3BocAWPm1oNXN5qkAV4QeDUmAPpkTcN1rrmCMWAfsXJY"));

    Data rawMessageData = {
        1,   0,  2,   3,   32, 123, 225, 60,  67,  196, 82,  133, 146, 234, 243, 253, 52, 224, 100,
        198, 65, 197, 190, 60, 182, 105, 24,  119, 215, 173, 233, 77,  255, 54,  115, 65, 8,   234,
        234, 48, 114, 60,  51, 181, 37,  7,   188, 84,  2,   73,  16,  97,  47,  136, 94, 76,  128,
        193, 11, 153, 160, 71, 253, 66,  192, 172, 186, 206, 0,   0,   0,   0,   0,   0,  0,   0,
        0,   0,  0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  0,   0,
        0,   0,  0,   0,   0,  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4,   4,
        4,   4,  4,   4,   4,  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4,   1,
        2,   2,  0,   1,   12, 2,   0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};

    std::vector<PrivateKey> signerKeys;
    signerKeys.push_back(privateKey);
    Data rawTransaction = Signer::signRawMessage(signerKeys, rawMessageData);

    Data expectedTransactionBytes = {
        1,   110, 127, 131, 73,  151, 123, 72,  43,  204, 240, 191, 194, 2,   173, 145, 114, 149,
        128, 56,  49,  229, 156, 203, 134, 91,  151, 214, 87,  70,  71,  145, 235, 254, 51,  54,
        135, 155, 132, 185, 241, 101, 228, 100, 163, 71,  81,  254, 48,  213, 75,  1,   243, 201,
        243, 63,  150, 154, 175, 225, 232, 89,  81,  177, 9,   1,   0,   2,   3,   32,  123, 225,
        60,  67,  196, 82,  133, 146, 234, 243, 253, 52,  224, 100, 198, 65,  197, 190, 60,  182,
        105, 24,  119, 215, 173, 233, 77,  255, 54,  115, 65,  8,   234, 234, 48,  114, 60,  51,
        181, 37,  7,   188, 84,  2,   73,  16,  97,  47,  136, 94,  76,  128, 193, 11,  153, 160,
        71,  253, 66,  192, 172, 186, 206, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
        4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   1,
        2,   2,   0,   1,   12,  2,   0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};
    ASSERT_EQ(rawTransaction, expectedTransactionBytes);
}

TEST(SolanaSigner, SignDelegateStake) {
    const auto privateKeyFrom =
        PrivateKey(Base58::bitcoin.decode("GGT4G41n1K3E4MTjb7VwADSFNJA3Jx7wUxm54Fpcje6w"));
    const auto publicKeyFrom = privateKeyFrom.getPublicKey(TWPublicKeyTypeED25519);
    const auto from = Address(publicKeyFrom);
    ASSERT_EQ(Data(publicKeyFrom.bytes.begin(), publicKeyFrom.bytes.end()),
              Base58::bitcoin.decode("CpKHKhhrfA3HWm1Tc7Nm8MWLvypXwrorHQGPpuygcmYP"));
    const auto privateKeyStake =
        PrivateKey(Base58::bitcoin.decode("2bwUDLUVYCfUhQHiAiwvHzM8oNT7pdk5J1XjhTLeumP5"));
    const auto publicKeyStake = privateKeyStake.getPublicKey(TWPublicKeyTypeED25519);
    const auto stakeAccount = Address(publicKeyStake);
    ASSERT_EQ(Data(publicKeyStake.bytes.begin(), publicKeyStake.bytes.end()),
              Base58::bitcoin.decode("5xVakzurz4ZVYcZSMYaP2kDaLRx7JekXKRp1XY3TrYyY"));

    const auto voteAccount = Address("FkL2bzbUbp3J9MQEX3toMBA4q8ZcHcjeacdtn2Ti8Qec");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto message = Message(from, stakeAccount, voteAccount, 42, recentBlockhash);
    auto transaction = Transaction(message);

    std::vector<PrivateKey> signerKeys;
    // Sign order should not matter
    signerKeys.push_back(privateKeyStake);
    signerKeys.push_back(privateKeyFrom);
    Signer::sign(signerKeys, transaction);

    std::vector<Signature> expectedSignatures;
    Signature expectedSignature0(
        "5J2dVJcLLxfvFWxooaEE81Bpz4qpaGwqSezWfQnP5F2kYvbhQjdQ23mADjYap2svT767bcUjigwf9KgWUtSSjcr5");
    expectedSignatures.push_back(expectedSignature0);
    Signature expectedSignature1(
        "4W1pvybhUgnUTf6Kuchm9x52TCmBSjFf21GPYTQDuaUR6zeuoJZiBFNhWjWbi3WDUvFp8F7eEEcwBXhAZHw32Gbo");
    expectedSignatures.push_back(expectedSignature1);
    ASSERT_EQ(transaction.signatures, expectedSignatures);

    Data expectedTransactionBytes = {
        2,   214, 192, 155, 86,  43,  203, 65,  232, 21,  242, 217, 163, 5,   17,  169, 50,  70,
        29,  245, 160, 199, 42,  14,  96,  43,  202, 132, 255, 81,  6,   125, 99,  156, 250, 166,
        59,  197, 110, 189, 146, 114, 115, 31,  136, 33,  169, 150, 87,  69,  171, 207, 226, 162,
        10,  243, 80,  88,  147, 60,  88,  135, 115, 154, 10,  175, 18,  96,  67,  123, 25,  16,
        205, 51,  44,  228, 165, 27,  10,  92,  10,  18,  84,  86,  20,  44,  14,  150, 171, 123,
        123, 16,  179, 69,  92,  249, 202, 144, 51,  164, 146, 35,  182, 110, 46,  56,  159, 9,
        147, 104, 155, 46,  193, 164, 140, 22,  159, 207, 11,  146, 5,   16,  243, 14,  24,  0,
        254, 166, 6,   2,   0,   5,   7,   175, 143, 134, 108, 1,   179, 182, 122, 138, 126, 223,
        218, 36,  36,  164, 134, 146, 16,  243, 97,  169, 141, 183, 130, 133, 135, 133, 111, 14,
        184, 239, 180, 73,  166, 2,   145, 8,   184, 74,  27,  252, 116, 189, 17,  204, 230, 224,
        205, 103, 45,  171, 233, 211, 153, 58,  2,   143, 63,  235, 221, 183, 24,  55,  115, 219,
        29,  27,  60,  1,   43,  59,  72,  110, 238, 229, 98,  127, 161, 92,  84,  236, 139, 51,
        200, 13,  212, 54,  215, 45,  15,  189, 205, 4,   83,  117, 105, 6,   167, 213, 23,  24,
        199, 116, 201, 40,  86,  99,  152, 105, 29,  94,  182, 139, 94,  184, 163, 155, 75,  109,
        92,  115, 85,  91,  33,  0,   0,   0,   0,   6,   161, 216, 23,  165, 2,   5,   11,  104,
        7,   145, 230, 206, 109, 184, 142, 30,  91,  113, 80,  246, 31,  198, 121, 10,  78,  180,
        209, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   6,   161, 216, 23,  145, 55,  84,  42,  152, 52,  55,  189, 254, 42,  122, 178, 85,
        127, 83,  92,  138, 120, 114, 43,  104, 164, 157, 192, 0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   5,   2,   0,   1,   52,  0,
        0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0,   96,  0,   0,   0,   0,   0,   0,
        0,   6,   161, 216, 23,  145, 55,  84,  42,  152, 52,  55,  189, 254, 42,  122, 178, 85,
        127, 83,  92,  138, 120, 114, 43,  104, 164, 157, 192, 0,   0,   0,   0,   6,   4,   1,
        2,   3,   4,   12,  0,   0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};
    ASSERT_EQ(transaction.serialize(), expectedTransactionBytes);
}
