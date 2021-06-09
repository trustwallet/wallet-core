// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ABI.h"
#include <HexCoding.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ethereum::ABI;

// https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts

TEST(EthereumAbiStructEncoder, encodeTypes) {
    ParametersNamed mailMessageFrom("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Cow")),
        std::make_shared<ParamNamed>("wallets", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
            std::make_shared<ParamAddress>(parse_hex("CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826")),
            std::make_shared<ParamAddress>(parse_hex("DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"))
        }))
    });
    ParametersNamed mailMessageTo0("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Bob")),
        std::make_shared<ParamNamed>("wallets", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
            std::make_shared<ParamAddress>(parse_hex("bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB")),
            std::make_shared<ParamAddress>(parse_hex("B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57")),
            std::make_shared<ParamAddress>(parse_hex("B0B0b0b0b0b0B000000000000000000000000000"))
        }))
    });
    ParametersNamed mailMessageGroup("Group", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("")),
        std::make_shared<ParamNamed>("members", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
            std::make_shared<ParametersNamed>(mailMessageFrom)
        }))
    });
    ParametersNamed mailMessage("Mail", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("from", std::make_shared<ParametersNamed>(mailMessageFrom)),
        std::make_shared<ParamNamed>("to", std::make_shared<ParamArray>(std::make_shared<ParametersNamed>(mailMessageTo0))),
        std::make_shared<ParamNamed>("contents", std::make_shared<ParamString>("Hello, Bob!"))
    });

    {
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "efa62530c7ae3a290f8a13a5fc20450bdb3a6af19d9d9d2542b5a94e631a9168"
        ))), "ca322beec85be24e374d18d582a6f2997f75c54e7993ab5bc07404ce176ca7cd");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "000000000000000000000000CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
            "000000000000000000000000DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
        ))), "8a8bfe642b9fc19c25ada5dadfd37487461dc81dd4b0778f262c163ed81b5e2a");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "0000000000000000000000000000000000000000000000000000000000000020"
            "0000000000000000000000000000000000000000000000000000000000000040"
            "0000000000000000000000000000000000000000000000000000000000000080"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "426f620000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "000000000000000000000000b0bdabea57b0bdabea57b0bdabea57b0bdabea57"
            "000000000000000000000000b0b0b0b0b0b0b000000000000000000000000000"
        ))), "1f73a5928b83b0d8e1b44a20b1427f108082ec510012578b940e468b1bee3d9e");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "0000000000000000000000000000000000000000000000000000000000000003"
            "426f620000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "000000000000000000000000b0bdabea57b0bdabea57b0bdabea57b0bdabea57"
            "000000000000000000000000b0b0b0b0b0b0b000000000000000000000000000"
        ))), "ea69137b7b47b5658d044db54c96bb39a88d7fa66b683fc3e54aa623d402a2ee");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "426f620000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000000003"
            "000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "000000000000000000000000b0bdabea57b0bdabea57b0bdabea57b0bdabea57"
            "000000000000000000000000b0b0b0b0b0b0b000000000000000000000000000"
        ))), "da2c7489a169b43e8095bd654c04cad2c8670489110f478ad552e1554e24da54");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex(
            "426f620000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "000000000000000000000000b0bdabea57b0bdabea57b0bdabea57b0bdabea57"
            "000000000000000000000000b0b0b0b0b0b0b000000000000000000000000000"
        ))), "287f5852a41186a43646e84fe23cbac27ab1c2f6cffe74d8ea55d590f473d4fb");
    }

    {
        EXPECT_EQ(mailMessageFrom.getType(), "Person");
        std::vector<std::string> il1;
        EXPECT_EQ(mailMessageFrom.getExtraTypes(il1), "Person(string name,address[] wallets)");
        EXPECT_EQ(mailMessageFrom.getTypeLong(), "Person(string name,address[] wallets)");

        EXPECT_EQ(mailMessageGroup.getType(), "Group");
        std::vector<std::string> il2;
        EXPECT_EQ(mailMessageGroup.getExtraTypes(il2), "Group(string name,Person[] members)Person(string name,address[] wallets)");
        EXPECT_EQ(mailMessageGroup.getTypeLong(), "Group(string name,Person[] members)");

        EXPECT_EQ(mailMessage.getType(), "Mail");
        std::vector<std::string> il3;
        EXPECT_EQ(mailMessage.getExtraTypes(il3), "Mail(Person from,Person[] to,string contents)Person(string name,address[] wallets)");
        EXPECT_EQ(mailMessage.getTypeLong(), "Mail(Person from,Person[] to,string contents)");
    }

    EXPECT_EQ(hex(mailMessageFrom.encodeHashes()),
        "fabfe1ed996349fc6027709802be19d047da1aa5d6894ff5f6486d92db2e6860"
        "8c1d2bd5348394761719da11ec67eedae9502d137e8940fee8ecd6f641ee1648"
        "8a8bfe642b9fc19c25ada5dadfd37487461dc81dd4b0778f262c163ed81b5e2a"
    );
    EXPECT_EQ(hex(mailMessageFrom.hashStruct()), "9b4846dd48b866f0ac54d61b9b21a9e746f921cefa4ee94c4c0a1c49c774f67f");

    EXPECT_EQ(hex(mailMessageTo0.encodeHashes()),
        "fabfe1ed996349fc6027709802be19d047da1aa5d6894ff5f6486d92db2e6860"
        "28cac318a86c8a0a6a9156c2dba2c8c2363677ba0514ef616592d81557e679b6"
        "d2734f4c86cc3bd9cabf04c3097589d3165d95e4648fc72d943ed161f651ec6d"
    );
    EXPECT_EQ(hex(mailMessageTo0.hashStruct()), "efa62530c7ae3a290f8a13a5fc20450bdb3a6af19d9d9d2542b5a94e631a9168");

    EXPECT_EQ(hex(mailMessage.encodeHashes()),
        "4bd8a9a2b93427bb184aca81e24beb30ffa3c747e2a33d4225ec08bf12e2e753"
        "9b4846dd48b866f0ac54d61b9b21a9e746f921cefa4ee94c4c0a1c49c774f67f"
        "ca322beec85be24e374d18d582a6f2997f75c54e7993ab5bc07404ce176ca7cd"
        "b5aadf3154a261abdd9086fc627b61efca26ae5702701d05cd2305f7c52a2fc8"
    );
    EXPECT_EQ(hex(mailMessage.hashStruct()), "eb4221181ff3f1a83ea7313993ca9218496e424604ba9492bb4052c03d5c3df8");


    {
        EXPECT_EQ(hex(data(std::string("Cow"))), "436f77");
        EXPECT_EQ(hex(Hash::keccak256(parse_hex("436f77"))), "8c1d2bd5348394761719da11ec67eedae9502d137e8940fee8ecd6f641ee1648");
    }
}
