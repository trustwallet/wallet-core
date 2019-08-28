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

    auto expectedHex =
        "02d6c09b562bcb41e815f2d9a30511a932461df5a0c72a0e602bca84ff51067d639cfaa63bc56ebd9272731f88"
        "21a9965745abcfe2a20af35058933c5887739a0aaf1260437b1910cd332ce4a51b0a5c0a125456142c0e96ab7b"
        "7b10b3455cf9ca9033a49223b66e2e389f0993689b2ec1a48c169fcf0b920510f30e1800fea60602000507af8f"
        "866c01b3b67a8a7edfda2424a4869210f361a98db7828587856f0eb8efb449a6029108b84a1bfc74bd11cce6e0"
        "cd672dabe9d3993a028f3febddb7183773db1d1b3c012b3b486eeee5627fa15c54ec8b33c80dd436d72d0fbdcd"
        "0453756906a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d817a502050b68"
        "0791e6ce6db88e1e5b7150f61fc6790a4eb4d10000000000000000000000000000000000000000000000000000"
        "0000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000000000"
        "0000000000000000000000000000000000000000000000000000000000020502000134000000002a0000000000"
        "0000600000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc000000000060401"
        "0203040c000000002a00000000000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}
