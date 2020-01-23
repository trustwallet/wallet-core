// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Action.h"
#include "FIO/Transaction.h"
#include "FIO/TransactionBuilder.h"

#include "HexCoding.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::FIO;
using namespace std;


Data chainId = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd77");

TEST(FIOTransactionBuilder, PackInternals) {
    ChainParams chainParams{chainId, 11565, 4281229859};
    PrivateKey privKey = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035")); // 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri
    PublicKey pubKey = privKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    Address addr(pubKey);
    string t = TransactionBuilder::createTx(addr, privKey, "adam@fiotestnet", {
        {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"},
        {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"},
        {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
        chainParams, 0, TransactionBuilder::WalletFioName, 1579729429);

    EXPECT_EQ("{\n"
        "\"signatures\": [\"SIG_K1_K85BxXzJwvjPs3mFeKatWSjBHuMXTw634RRtf6ZMytpzLCdpHcJ7CQWPeXJvwm7aoz7XJJKapmoT4jzCLoVBv2cxP149Bx\"],\n"
        "\"compression\": \"none\",\n"
        "\"packed_context_free_data\": \"\",\n"
        "\"packed_trx\": \""
        "15c2285e2d2d23622eff0000000001003056372503a85b0000c6eaa664523201102b2f46fca756b200000000a8ed3232bd010f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b39730000000000000000102b2f46fca756b20e726577617264734077616c6c657400"
        "\"\n}", t);
}

TEST(FIOTransaction, ActionAddAddress) {
    AddAddressData aadata("adam@fiotestnet", {
        {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"},
        {"ETH", "0xce5cB6c92Da37bbBa91Bd40D4C9D4D724A3a8F51"},
        {"BNB", "bnb1ts3dg54apwlvr9hupv2n0j6e46q54znnusjk9s"}},
        0, "rewards@wallet", "hhq2g4qgycfb");
    Data ser1;
    aadata.serialize(ser1);
    EXPECT_EQ(
        hex(parse_hex("0F6164616D4066696F746573746E657403034254432A626331717679343037347267676B647232707A773576706E6E3632656730736D7A6C7877703730643776034554482A30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424E422A626E6231747333646735346170776C76723968757076326E306A366534367135347A6E6E75736A6B397300000000000000007016F2CC12266C6B0E726577617264734077616C6C6574")),
        hex(ser1));

    Action aaAction;
    aaAction.account = "fio.address";
    aaAction.name = "addaddress";
    aaAction.actionDataSer = ser1;
    aaAction.auth.authArray.push_back(Authorization{"hhq2g4qgycfb", "active"});
    Data ser2;
    aaAction.serialize(ser2);
    EXPECT_EQ(
        "003056372503a85b0000c6eaa66452320"
        "17016f2cc12266c6b00000000a8ed3232"
        "bd01"
        "0f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b397300000000000000007016f2cc12266c6b0e726577617264734077616c6c657400",
        hex(ser2));

    Transaction tx;
    tx.expiration = 1579729429; //1579729428729;
    tx.refBlockNumber = 11565;
    tx.refBlockPrefix = 4281229859;
    tx.actions.push_back(aaAction);
    Data ser3;
    tx.serialize(ser3);
    EXPECT_EQ(
        "15c2285e2d2d23622eff0000000001"
        "003056372503a85b0000c6eaa66452320"
        "17016f2cc12266c6b00000000a8ed3232"
        "bd01"
        "0f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b397300000000000000007016f2cc12266c6b0e726577617264734077616c6c657400",
        hex(ser3));
}
