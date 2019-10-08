// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "HexCoding.h"
#include "TWTestUtilities.h"
#include "proto/Solana.pb.h"

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWSolanaAddress.h>
#include <TrustWalletCore/TWSolanaProto.h>
#include <TrustWalletCore/TWSolanaSigner.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWSolanaSigner, SignTransfer) {
    auto privateKey = Base58::bitcoin.decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_transfer_transaction();
    message.set_private_key(privateKey.data(), privateKey.size());
    message.set_recipient("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    message.set_value((uint64_t)42L);
    input.set_recent_blockhash("11111111111111111111111111111111");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWSolanaSignerSign(inputTWData);
    auto output = Solana::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto expectedHex =
        "01fda1c8ad8872d94f7eab52f9c38dc77e1061f4897e3de2b8469eb0992269f6fa1f173e93dbb2da738ab4e895"
        "9ffa50cd087cdfa889f3a1b8acdd62552f7c1d070100020366c2f508c9c555cacc9fb26d88e88dd54e210bb5a8"
        "bce5687f60d7e75c4cd07fc68b3c894c782b05a9c27fc6c66eb14d4e7d31de9086ab7d2129bcb0493afa020000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000001020200010c020000002a00000000000000";
    ASSERT_EQ(hex(output.encoded()), expectedHex);
}

TEST(TWSolanaSigner, SignDelegateStakeTransaction) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_stake_transaction();
    message.set_private_key(privateKey.data(), privateKey.size());
    message.set_vote_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message.set_value((uint64_t)42L);
    message.set_stake_pubkey("Bqa7hbY1McviVybz8pyBZEDcJRuy6ZYen3XjAh6VLcsk");
    input.set_recent_blockhash("11111111111111111111111111111111");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWSolanaSignerSign(inputTWData);
    auto output = Solana::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

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
    ASSERT_EQ(hex(output.encoded()), expectedHex);
}

TEST(TWSolanaSigner, RandomStakePubkey) {
    auto privateKey = Base58::bitcoin.decode("AevJ4EWcvQ6dptBDvF2Ri5pU6QSBjkzSGHMfbLFKa746");
    auto input0 = Solana::Proto::SigningInput();
    auto &message0 = *input0.mutable_stake_transaction();
    message0.set_private_key(privateKey.data(), privateKey.size());
    message0.set_vote_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message0.set_value((uint64_t)42L);
    input0.set_recent_blockhash("11111111111111111111111111111111");
    auto inputData0 = input0.SerializeAsString();
    auto inputTWData0 = TWDataCreateWithBytes((const byte *)inputData0.data(), inputData0.size());
    auto outputTWData0 = TWSolanaSignerSign(inputTWData0);
    auto output0 = Solana::Proto::SigningOutput();
    output0.ParseFromArray(TWDataBytes(outputTWData0), TWDataSize(outputTWData0));

    auto input1 = Solana::Proto::SigningInput();
    auto &message1 = *input1.mutable_stake_transaction();
    message1.set_private_key(privateKey.data(), privateKey.size());
    message1.set_vote_pubkey("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    message1.set_value((uint64_t)42L);
    input1.set_recent_blockhash("11111111111111111111111111111111");
    auto inputData1 = input1.SerializeAsString();
    auto inputTWData1 = TWDataCreateWithBytes((const byte *)inputData1.data(), inputData1.size());
    auto outputTWData1 = TWSolanaSignerSign(inputTWData1);
    auto output1 = Solana::Proto::SigningOutput();
    output1.ParseFromArray(TWDataBytes(outputTWData1), TWDataSize(outputTWData1));

    ASSERT_NE(hex(output0.encoded()), hex(output1.encoded()));
}

TEST(TWSolanaSigner, SignDeactivateStakeTransaction) {
    auto privateKey = Base58::bitcoin.decode("5PcaDJjTMnZEqJzayijWhYJAbUuURjtkJq8Zi2HD2k7Q");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_deactivate_stake_transaction();
    message.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWSolanaSignerSign(inputTWData);
    auto output = Solana::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto expectedHex =
        "019a5534012ae734086fdcef5e7838c88a025ee2dd96b945753bdd5d90d9b43663b47f674e20835759cb"
        "7c50a3238db1f1d961be53f7b4d8cb989b003d16163c0c01000304bb25296230c34d03f077f3eb35c4b4"
        "a2ca436bef2e4628b77a32bb743d6df8ea962bdd5edcbc6f93841f68ade86a279756734dbb921b6d1cf1"
        "aa5460dbe40f2806a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a1d8"
        "179137542a983437bdfe2a7ab2557f535c8a78722b68a49dc00000000000000000000000000000000000"
        "000000000000000000000000000000000000000103030001020403000000";
    ASSERT_EQ(hex(output.encoded()), expectedHex);
}

TEST(TWSolanaSigner, SignWithdrawStakeTransaction) {
    auto stakePrivateKey = Base58::bitcoin.decode("5PcaDJjTMnZEqJzayijWhYJAbUuURjtkJq8Zi2HD2k7Q");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_withdraw_transaction();
    message.set_private_key(stakePrivateKey.data(), stakePrivateKey.size());
    message.set_recipient("C3e7ryQjYJFSUetohBofTaWEBbNcq4yVX43hi7igVtcP");
    message.set_value((uint64_t)42L);
    input.set_recent_blockhash("11111111111111111111111111111111");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWSolanaSignerSign(inputTWData);
    auto output = Solana::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    auto expectedHex =
        "010ffa019ccce82c7cbed48f76897145e0ce726f9e73aaf49bd301c3ba0635e840f54838df6e3378a3d2"
        "39c017783251c00f030f73f7dca1c8a64e6b3469eecc0101000405bb25296230c34d03f077f3eb35c4b4"
        "a2ca436bef2e4628b77a32bb743d6df8eaa41db0070ae3aee30c6ce259f81a49cd15603fe29b47ba80ac"
        "734ff3911462a006a7d51718c774c928566398691d5eb68b5eb8a39b4b6d5c73555b210000000006a7d5"
        "17193584d0feed9bb3431d13206be544281b57b8566cc5375ff400000006a1d8179137542a983437bdfe"
        "2a7ab2557f535c8a78722b68a49dc0000000000000000000000000000000000000000000000000000000"
        "000000000000000000010404000102030c020000002a00000000000000";
    ASSERT_EQ(hex(output.encoded()), expectedHex);
}

TEST(TWSolanaAddress, HDWallet) {
    auto mnemonic =
        "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(
        TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeDerivationPath(TWCoinTypeSolana));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = TWSolanaAddressCreateWithPublicKey(publicKey);
    auto addressStr = WRAPS(TWSolanaAddressDescription(address));

    assertStringsEqual(addressStr, "2bUBiBNZyD29gP1oV6de7nxowMLoDBtopMMTGgMvjG5m");
}
