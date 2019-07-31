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

TEST(SolanaTransaction, MessageData) {
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

TEST(SolanaTransaction, SerializeTransaction) {
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
