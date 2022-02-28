// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Actor.h"
#include "FIO/Signer.h"

#include "HexCoding.h"
#include "Hash.h"
#include "Base58.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FIO;
using namespace std;


TEST(FIOSigner, SignEncode) {
    string sig1 = Signer::signatureToBsase58(parse_hex("1f4fccc30bcba876963aef6de584daf7258306c02f4528fe25b116b517de8b349968bdc080cd6bef36f5a46d31a7c01ed0806ad215bb66a94f61e27a895d610983"));;
    EXPECT_EQ("SIG_K1_K5hJTPeiV4bDkNR13mf66N2DY5AtVL4NU1iWE4G4dsczY2q68oCcUVxhzFdxjgV2eAeb2jNV1biqtCJ3SaNL8kkNgoZ43H", sig1);
}

TEST(FIOSigner, SignInternals) {
    // 5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf
    PrivateKey pk = PrivateKey(parse_hex("ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035"));
    {
        Data pk2 = parse_hex("80");
        append(pk2, pk.bytes);
        EXPECT_EQ("5KEDWtAUJcFX6Vz38WXsAQAv2geNqT7UaZC8gYu9kTuryr3qkri", Base58::bitcoin.encodeCheck(pk2));
    }
    Data rawData = parse_hex("4e46572250454b796d7296eec9e8896327ea82dd40f2cd74cf1b1d8ba90bcd774a26285e19fac10ac5390000000001003056372503a85b0000c6eaa6645232017016f2cc12266c6b00000000a8ed3232bd010f6164616d4066696f746573746e657403034254432a626331717679343037347267676b647232707a773576706e6e3632656730736d7a6c7877703730643776034554482a30786365356342366339324461333762624261393142643430443443394434443732344133613846353103424e422a626e6231747333646735346170776c76723968757076326e306a366534367135347a6e6e75736a6b397300000000000000007016f2cc12266c6b0e726577617264734077616c6c6574000000000000000000000000000000000000000000000000000000000000000000");
    Data hash = Hash::sha256(rawData);
    EXPECT_EQ("0f3cca0f50da4200b2858f65de1ea4530a9afd9e4bfc0b6b7196e36c25cc7a8b", hex(hash));
    Data sign2 = Signer::signData(pk, rawData);
    EXPECT_EQ("1f4ae8d1b993f94d0de4b249d5185481770de0711863ad640b3aac21de598fcc02761c6e5395106bafb7b09aab1c7aa5ac0573dbd821c2d255725391a5105d30d1", hex(sign2));

    string sigStr = Signer::signatureToBsase58(sign2);
    EXPECT_EQ("SIG_K1_K54CA1jmhgWrSdvrNrkokPyvqh7dwsSoQHNU9xgD3Ezf6cJySzhKeUubVRqmpYdnjoP1DM6SorroVAgrCu3qqvJ9coAQ6u", sigStr);
    EXPECT_TRUE(Signer::verify(pk.getPublicKey(TWPublicKeyTypeSECP256k1), hash, sign2));
}

TEST(FIOSigner, Actor) {
    {
        const auto addr1 = "FIO6cDpi7vPnvRwMEdXtLnAmFwygaQ8CzD7vqKLBJ2GfgtHBQ4PPy";
        Address addr = Address(addr1);
        EXPECT_EQ(addr1, addr.string());

        uint64_t shortenedKey = Actor::shortenKey(addr.bytes);
        EXPECT_EQ(1518832697283783336U, shortenedKey);
        string name = Actor::name(shortenedKey);
        EXPECT_EQ("2odzomo2v4pec", name);
    }
    const int n = 4;
    const string addrArr[n] = {
        "FIO6cDpi7vPnvRwMEdXtLnAmFwygaQ8CzD7vqKLBJ2GfgtHBQ4PPy",
        "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE",
        "FIO7bxrQUTbQ4mqcoefhWPz1aFieN4fA9RQAiozRz7FrUChHZ7Rb8",
        "FIO6m1fMdTpRkRBnedvYshXCxLFiC5suRU8KDfx8xxtXp2hntxpnf",
    };
    const string actorArr[n] = {
        "2odzomo2v4pe",
        "hhq2g4qgycfb",
        "5kmx4qbqlpld",
        "qdfejz2a5wpl",
    };
    for (int i = 0; i < n; ++i) {
        Address addr = Address(addrArr[i]);
        EXPECT_EQ(addrArr[i], addr.string());

        string actor = Actor::actor(addr);
        EXPECT_EQ(actorArr[i], actor);
    }
}
