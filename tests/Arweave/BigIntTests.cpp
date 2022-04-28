// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Arweave/BigInt.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW::Arweave;
using namespace TW;


const auto Random1 = "f5e1bb6bc4b9e265ce1dbd98ba928529f1426bbf4cb5dca03c5fcc23f5136842b985ff0f2dc9d7aa8152bfea346c11476212a6355073558dc51891edd5d9f9a23ef1035182c91f1b8cb1fd40663e27e78a8b3bf24a687fd50034afb164b430595f58b6da27164f4619786493e56ac1be42178b31317dbb0f42f8af325f0a0802906a449d1a63db8348fa1dc0aee3d4a22dcb5d841fe54ca969eea70abbca373889b5ccc98ce8d6fbbb3860a74c8363230718b7066575cc4b0e407025dddfe6a06bb7c70b1151588b32b4ff8a9e5228f050a402cf44688f4976d6a035223252a39f89a56711dcedc03baed24a6b09b6e540b766c5bc1c6188b57048cd3b92fa5546"; // 245 225 187 ...

TEST(ArweaveBigInt, bigIntFromData) {
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromData(parse_hex("00"))), "0");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromData(parse_hex("01"))), "1");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromData(parse_hex("ff"))), "255");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromData(parse_hex("0100"))), "256");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromData(parse_hex("01020304"))), "16909060");

    const auto d = parse_hex(Random1);
    const auto num = bigIntFromData(d);
    EXPECT_EQ(boost::lexical_cast<std::string>(num), "7946162526813706880372336986945905313939024240726515935060607654332853150814982697321009610678200264798080328970721024790880855702596590516619295343111924877949472398575198731290708137329699061115105777813138506502232828085716805985988794779315184777909871921129492794463570571108771203437134823948270475653516728038891538126307124402933455112433932177841125213702925150513704138212507726744214504825439455246557576190980964671804659161199899369285539541717395498258588054761130948128949663698667707946701622532934264698979812063542330893326743686647341911716029437184447972691904751493773787500281887519273710740198726");
    EXPECT_EQ(num % 256, 70);
    EXPECT_EQ(num % 30, 6);
}

TEST(ArweaveBigInt, bigIntFromDataWithSign) {
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("00"))), "0");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("01"))), "0");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("0001"))), "1");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("ff01"))), "1");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("0001020304"))), "16909060");
    EXPECT_EQ(boost::lexical_cast<std::string>(bigIntFromDataWithSign(parse_hex("ff01020304"))), "16909060");
    EXPECT_EQ(bigIntFromDataWithSign(parse_hex("")), 0); // invalid input
    EXPECT_EQ(bigIntFromDataWithSign(parse_hex("00")), 0); // invalid input
    EXPECT_EQ(bigIntFromDataWithSign(parse_hex("123")), 0); // invalid input
    {
        const auto d = parse_hex(Random1);
        const auto num = bigIntFromDataWithSign(d);
        EXPECT_EQ(boost::lexical_cast<std::string>(num), "28496039342510091697192198221767083630219086646222347188673024589312641792349428487695264855086466194344428544856010014821563710726314848322698008432095513394907443586454688811909688484234294950071656409041952274418734518274965726338461666388523012617368687380598058956114477681210080328284486162907531200647716358422794233239305580225135586194169049186176309473862763977602720998934949777401149573716061648701393279957809619193063128885996122706633626475240126607942262006979996746286173775569533696630379600673373704893542732795846226963940020881929764507846914003306719553801724235560511192860703384564109663688006");
        EXPECT_EQ(num % 256, 70);
        EXPECT_EQ(num % 30, 16);
    }
}

TEST(ArweaveBigInt, BigIntTestBit) {
    EXPECT_TRUE(bigIntTestBit(65535, 15));
    EXPECT_FALSE(bigIntTestBit(65535, 16));
    EXPECT_TRUE(bigIntTestBit(65535, 14));
    EXPECT_TRUE(bigIntTestBit(65535, 13));
    EXPECT_TRUE(bigIntTestBit(65535, 1));
    EXPECT_TRUE(bigIntTestBit(65535, 0));

    EXPECT_FALSE(bigIntTestBit(65536, 15));
    EXPECT_TRUE(bigIntTestBit(65536, 16));
    EXPECT_FALSE(bigIntTestBit(65536, 14));
    EXPECT_FALSE(bigIntTestBit(65536, 13));
    EXPECT_FALSE(bigIntTestBit(65536, 1));
    EXPECT_FALSE(bigIntTestBit(65536, 0));
}
