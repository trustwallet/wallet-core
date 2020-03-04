// Copyright © 2017-2020 Trust Wallet.
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
        "0100010353f9d600fe925083bb399907ea648d23a6a081fc7e9059202fd725f7edd281dd3cc1ff9ba3c7a876c8"
        "082df2f8a36ea9342ce3819dd4b6fa72d4a18e04a5363a00000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000010202"
        "00010c020000002a00000000000000";
    ASSERT_EQ(hex(transaction.messageData()), expectedHex);
}

TEST(SolanaTransaction, TransferSerializeTransaction) {
    auto from = Address("41a5jYky56M6EWDsFfLaZRxoRtgAJSRWxJnxaJNJELn5");
    auto to = Address("4iSnyfDKaejniaPc2pBBckwQqV3mDS93go15NdxWJq2y");
    Solana::Hash recentBlockhash("11111111111111111111111111111111");
    auto transaction = Transaction(from, to, 42, recentBlockhash);
    Signature signature(
        "46SRiQGvtPb1iivDfnuC3dW1GzXkfQPTjdUyvFqF2sdPvFrsfx94fys2xpNKR6UiAj7RgKWdJG6mEfe85up6i1JT");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedHex =
        "019abda83fb68c77c698cba9ae8e7c21b06cbe39aca3bb34deb3dbcd061683bd47371d2eec06417e203ee19d88"
        "672819ee3319e7089ee0117128d95bf5ba57460c010001032cb8775e08dca27a9b723611f42d474c5e809df576"
        "e242816cfee4bfc56835da373109b0e40ba3d915ac3ae2417dfd0544369bf31f5ce6e3b8329cdf4e6b058a0000"
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
        "4Q3ctvLybffZJBGdvrnEg171AjZeiMPGf3rVfGCpDVZRq9QaKVw8jWTMqHzZckGi2yXbGoQdBgeua24iMfPGHTtV");
    transaction.signatures.clear();
    transaction.signatures.push_back(signature);

    auto expectedHex =
        "01a9ec9c553c565d232ac98892a1d926ccedb50b85ea227f3d2acef5a5e203573de157d843e578f0c01f2d9f83"
        "dd70e6adcc396ee474626e3168bb6b93176be602010006080eba44e56f060007284dc037275a15094c1d6c0697"
        "ddb28b2be661dfb0f4bab857a6b8aba83da905fff9c3e62acc748cf497bd503f11b7f9c337b7f35346df0306a7"
        "d517192c5c51218cc94c3d4af17f58daee089ba1fd44e3dbd98a00000000378ba8d9f9881e9be69cf1d70ee0a9"
        "3ed0378b83203f42fa29f9df5c887f1c0d06a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b21"
        "0000000006a1d817a502050b680791e6ce6db88e1e5b7150f61fc6790a4eb4d100000000000000000000000000"
        "000000000000000000000000000000000000000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78"
        "722b68a49dc0000000000000000000000000000000000000000000000000000000000000000000000000030602"
        "00017c030000000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab8200000000000"
        "0000346a707754717431715a6f52377536753633397a32416e675946474e336e616b2a00000000000000d80700"
        "000000000006a1d8179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc0000000000702010274000000"
        "000eba44e56f060007284dc037275a15094c1d6c0697ddb28b2be661dfb0f4bab80eba44e56f060007284dc037"
        "275a15094c1d6c0697ddb28b2be661dfb0f4bab800000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000070501030405000402000000";
    ASSERT_EQ(hex(transaction.serialize()), expectedHex);
}
