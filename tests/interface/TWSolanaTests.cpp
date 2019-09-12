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
    auto fromPrivateKey = Base58::bitcoin.decode("GGT4G41n1K3E4MTjb7VwADSFNJA3Jx7wUxm54Fpcje6w");
    auto stakePrivateKey = Base58::bitcoin.decode("2bwUDLUVYCfUhQHiAiwvHzM8oNT7pdk5J1XjhTLeumP5");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_stake_transaction();
    message.set_from_private_key(fromPrivateKey.data(), fromPrivateKey.size());
    message.set_stake_private_key(stakePrivateKey.data(), stakePrivateKey.size());
    message.set_vote_pubkey("FkL2bzbUbp3J9MQEX3toMBA4q8ZcHcjeacdtn2Ti8Qec");
    message.set_value((uint64_t)42L);
    input.set_recent_blockhash("11111111111111111111111111111111");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWSolanaSignerSign(inputTWData);
    auto output = Solana::Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

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
    ASSERT_EQ(hex(output.encoded()), expectedHex);
}

TEST(TWSolanaSigner, SignDeactivateStakeTransaction) {
    auto stakePrivateKey = Base58::bitcoin.decode("5PcaDJjTMnZEqJzayijWhYJAbUuURjtkJq8Zi2HD2k7Q");
    auto input = Solana::Proto::SigningInput();

    auto &message = *input.mutable_deactivate_stake_transaction();
    message.set_private_key(stakePrivateKey.data(), stakePrivateKey.size());
    message.set_vote_pubkey("B7Cx2wYAry78VNR8uoewzFDq3FRKJh8exNMyzrpQSfLB");
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
