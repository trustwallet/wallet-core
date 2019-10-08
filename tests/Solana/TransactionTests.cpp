// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
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

    auto expectedHex =
        "0100020353f9d600fe925083bb399907ea648d23a6a081fc7e9059202fd725f7edd281dd3cc1ff9ba3c7a876c8"
        "082df2f8a36ea9342ce3819dd4b6fa72d4a18e04a5363a00000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000010202"
        "00010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.messageData()), expectedHex);
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

    auto expectedHex =
        "01d813c70630e70f8a57a47f4bf63cf3eb0e43b9f2daa43bd4dfcbefd562dddd06d8dc37ad7bd941edd2c5c91f"
        "e94d92ed6a91a39cecf231f007ea4ff0c358c10601000203a5b1d862445be46611868cd435ab63c9d5dbb403c2"
        "918ac7f24c4180d70c02f5bfed63ee22636b105131aba9ad2667a9fe345edbff08cdc22fa3fab6e20fdbf20000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000001020200010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}

TEST(SolanaTransaction, StakeSerializeTransaction) {
    auto signer = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto stakeAccount = Address("Bqa7hbY1McviVybz8pyBZEDcJRuy6ZYen3XjAh6VLcsk");
    auto voteAccount = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto message = Message(signer, stakeAccount, voteAccount, 42, recentBlockhash);
    auto transaction = Transaction(message);
    Signature signature(
        "2QLJoEpoV27YYTCDTg8bmWCeP3BELtBqwdyu34bGm6nexXJUdVUUSzePxpeUjFzdbpyvsTKajgyCNW8CXXct7f6L");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedHex =
        "0146236d3c6553ad8120c3ac0e97a1bb75ec2ceebe388982bf01a14e8b0d9c3453415120a0db0fa1a1125d88e9"
        "c12219d6660560338261e957dcadd9ac4afda40d010005080eba44e56f060007284dc037275a15094c1d6c0697"
        "ddb28b2be661dfb0f4bab8a106312279be880f9aef61c849c4ec109af4766f3fdcbe7db3ff48c06e9f90c306a7"
        "d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a9"
        "3ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b21"
        "0000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000"
        "000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78"
        "722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602"
        "000134000000002a00000000000000d80600000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78"
        "722b68a49dc000000000070201026c000000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be6"
        "61dfb0f4bab80eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab800000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000007050103040500040200000"
        "0";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}
