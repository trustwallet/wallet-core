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
    auto voteAddress = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address(STAKE_ADDRESS);
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto stakeAddress = addressFromValidatorSeed(signer, voteAddress, programId);
    auto message = Message(signer, stakeAddress, voteAddress, 42, recentBlockhash);
    auto transaction = Transaction(message);
    Signature signature(
        "4oUMPVJ6NHGX1DFaxnDKSyZkxuN4osjqZE4mzA4iiNHd9kub3RtVKi3SUvRPsB5x2kB6EuRmBcWR4nbiKeoKF9os");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

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
