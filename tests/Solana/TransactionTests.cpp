// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PublicKey.h"
#include "Solana/Address.h"
#include "Solana/Transaction.h"

#include "BinaryCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Solana;

TEST(SolanaTransaction, TransferMessageData) {
    auto from = Address("6eoo7i1khGhVm8tLBMAdq4ax2FxkKP4G7mCcfHyr3STN");
    auto to = Address("56B334QvCDMSirsmtEJGfanZm8GqeQarrSjdAb2MbeNM");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    Data expected_message = {
        1,   0,   2,  3,   83,  249, 214, 0,   254, 146, 80,  131, 187, 57,  153, 7,   234,
        100, 141, 35, 166, 160, 129, 252, 126, 144, 89,  32,  47,  215, 37,  247, 237, 210,
        129, 221, 60, 193, 255, 155, 163, 199, 168, 118, 200, 8,   45,  242, 248, 163, 110,
        169, 52,  44, 227, 129, 157, 212, 182, 250, 114, 212, 161, 142, 4,   165, 54,  58,
        0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,   2,   0,
        1,   12,  2,  0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};
    ASSERT_EQ(transaction.messageData(), expected_message);
}

TEST(SolanaTransaction, TransferSerializeTransaction) {
    auto from = Address("C9oZKzj7ajgu9vgV1evHrwrN6Yw22LtpwXEKoS4omZoi");
    auto to = Address("DvCpXoxoSAyoUgHWEe8M6LaD225798tYgFE7T3r4Z2iu");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    Signature signature(
        "5KZjkbW2KD1rViggHNoU4YqBVhfmpBY6YY5obM9qjSfC5QH1uFWUEbWgUPtrREEdyP3LLEjjrd1XhXVTSk34ma4d");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);
    Data expected_transaction_bytes = {
        1,   216, 19,  199, 6,   48,  231, 15,  138, 87,  164, 127, 75,  246, 60,  243, 235, 14,
        67,  185, 242, 218, 164, 59,  212, 223, 203, 239, 213, 98,  221, 221, 6,   216, 220, 55,
        173, 123, 217, 65,  237, 210, 197, 201, 31,  233, 77,  146, 237, 106, 145, 163, 156, 236,
        242, 49,  240, 7,   234, 79,  240, 195, 88,  193, 6,   1,   0,   2,   3,   165, 177, 216,
        98,  68,  91,  228, 102, 17,  134, 140, 212, 53,  171, 99,  201, 213, 219, 180, 3,   194,
        145, 138, 199, 242, 76,  65,  128, 215, 12,  2,   245, 191, 237, 99,  238, 34,  99,  107,
        16,  81,  49,  171, 169, 173, 38,  103, 169, 254, 52,  94,  219, 255, 8,   205, 194, 47,
        163, 250, 182, 226, 15,  219, 242, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
        2,   2,   0,   1,   12,  2,   0,   0,   0,   42,  0,   0,   0,   0,   0,   0,   0};
    ASSERT_EQ(transaction.serialize(), expected_transaction_bytes);
}

TEST(SolanaTransaction, StakeSerializeTransaction) {
    auto from = Address("CpKHKhhrfA3HWm1Tc7Nm8MWLvypXwrorHQGPpuygcmYP");
    auto stakeAccount = Address("5xVakzurz4ZVYcZSMYaP2kDaLRx7JekXKRp1XY3TrYyY");
    auto voteAccount = Address("FkL2bzbUbp3J9MQEX3toMBA4q8ZcHcjeacdtn2Ti8Qec");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto message = Message(from, stakeAccount, voteAccount, 42, recentBlockhash);
    auto transaction = Transaction(message);
    Signature signature0(
        "5J2dVJcLLxfvFWxooaEE81Bpz4qpaGwqSezWfQnP5F2kYvbhQjdQ23mADjYap2svT767bcUjigwf9KgWUtSSjcr5");
    Signature signature1(
        "4W1pvybhUgnUTf6Kuchm9x52TCmBSjFf21GPYTQDuaUR6zeuoJZiBFNhWjWbi3WDUvFp8F7eEEcwBXhAZHw32Gbo");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature0);
    transaction.signatures.push_back(signature1);
    Data expected_transaction_bytes = {
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
    ASSERT_EQ(transaction.serialize(), expected_transaction_bytes);
}
