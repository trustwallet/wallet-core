// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ABI.h"
#include "Ethereum/Address.h"
#include <HexCoding.h>
#include <PrivateKey.h>

#include <gtest/gtest.h>

using namespace TW::Ethereum::ABI;
using namespace TW::Ethereum;
using namespace TW;

// https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts

ParamStruct msgPersonCow2("Person", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Cow")),
    std::make_shared<ParamNamed>("wallets", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(parse_hex("CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826")),
        std::make_shared<ParamAddress>(parse_hex("DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"))
    }))
});
ParamStruct msgPersonBob3("Person", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Bob")),
    std::make_shared<ParamNamed>("wallets", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(parse_hex("bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB")),
        std::make_shared<ParamAddress>(parse_hex("B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57")),
        std::make_shared<ParamAddress>(parse_hex("B0B0b0b0b0b0B000000000000000000000000000"))
    }))
});
ParamStruct msgGroup("Group", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("")),
    std::make_shared<ParamNamed>("members", std::make_shared<ParamArray>(std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamStruct>(msgPersonCow2)
    }))
});
ParamStruct msgMailCow1Bob1("Mail", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("from", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Cow")),
        std::make_shared<ParamNamed>("wallet", std::make_shared<ParamAddress>(parse_hex("CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826")))
    })),
    std::make_shared<ParamNamed>("to", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Bob")),
        std::make_shared<ParamNamed>("wallet", std::make_shared<ParamAddress>(parse_hex("bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB")))
    })),
    std::make_shared<ParamNamed>("contents", std::make_shared<ParamString>("Hello, Bob!"))
});
ParamStruct msgMailCow2Bob3("Mail", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("from", std::make_shared<ParamStruct>(msgPersonCow2)),
    std::make_shared<ParamNamed>("to", std::make_shared<ParamArray>(std::make_shared<ParamStruct>(msgPersonBob3))),
    std::make_shared<ParamNamed>("contents", std::make_shared<ParamString>("Hello, Bob!"))
});
ParamStruct msgEIP712Domain("EIP712Domain", std::vector<std::shared_ptr<ParamNamed>>{
    std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Ether Mail")),
    std::make_shared<ParamNamed>("version", std::make_shared<ParamString>("1")),
    std::make_shared<ParamNamed>("chainId", std::make_shared<ParamUInt256>(1)),
    std::make_shared<ParamNamed>("verifyingContract", std::make_shared<ParamAddress>(parse_hex("CcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC")))
});

PrivateKey privateKeyCow = PrivateKey(Hash::keccak256(TW::data("cow")));

// See 'signedTypeData' in https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts
TEST(EthereumAbiStruct, encodeTypes) {
    EXPECT_EQ(msgMailCow1Bob1.encodeType(), "Mail(Person from,Person to,string contents)Person(string name,address wallet)");

    EXPECT_EQ(hex(msgMailCow1Bob1.hashType()), "a0cedeb2dc280ba39b857546d74f5549c3a1d7bdc2dd96bf881f76108e23dac2");

    EXPECT_EQ(hex(msgMailCow1Bob1.encodeHashes()), "a0cedeb2dc280ba39b857546d74f5549c3a1d7bdc2dd96bf881f76108e23dac2fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8cd54f074a4af31b4411ff6a60c9719dbd559c221c8ac3492d9d872b041d703d1b5aadf3154a261abdd9086fc627b61efca26ae5702701d05cd2305f7c52a2fc8");

    EXPECT_EQ(hex(msgMailCow1Bob1.hashStruct()), "c52c0ee5d84264471806290a3f2c4cecfc5490626bf912d01f240d7a274b371e");

    EXPECT_EQ(hex(msgEIP712Domain.hashStruct()), "f2cee375fa42b42143804025fc449deafd50cc031ca257e0b194a650a912090f");

    Address address = Address(privateKeyCow.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    EXPECT_EQ(address.string(), "0xCD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826");

    // TODO sig
    // 0xbe609aee343fb3c4b28e1df9e632fca64fcfaede20f02e86244efddf30957bd2
    // 0x4355c47d63924e8a72e509b65029052eb6c299d53a04e167c5775fd466751c9d07299936d304c153f6443dfa05f40ff007d72911b6f72307f996231605b915621c
}

TEST(EthereumAbiStruct, encodeTypes_Json) {
    auto hash = ParamStruct::hashStructJson("Mail",
        R"({
            "from": {"name": "Cow", "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"},
            "to": {"name": "Bob", "wallet": "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB"},
            "contents": "Hello, Bob!"
        })",
        R"([
            {"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]},
            {"Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person"}, {"name": "contents", "type": "string"}]}
        ])");
    ASSERT_EQ(hex(hash), "c52c0ee5d84264471806290a3f2c4cecfc5490626bf912d01f240d7a274b371e");
}

// See 'signedTypeData with V3 string' in https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts
TEST(EthereumAbiStruct, encodeTypes_v3) {
    EXPECT_EQ(msgMailCow1Bob1.encodeType(), "Mail(Person from,Person to,string contents)Person(string name,address wallet)");

    EXPECT_EQ(hex(msgMailCow1Bob1.hashType()), "a0cedeb2dc280ba39b857546d74f5549c3a1d7bdc2dd96bf881f76108e23dac2");

    EXPECT_EQ(hex(msgMailCow1Bob1.encodeHashes()), "a0cedeb2dc280ba39b857546d74f5549c3a1d7bdc2dd96bf881f76108e23dac2fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8cd54f074a4af31b4411ff6a60c9719dbd559c221c8ac3492d9d872b041d703d1b5aadf3154a261abdd9086fc627b61efca26ae5702701d05cd2305f7c52a2fc8");

    EXPECT_EQ(hex(msgMailCow1Bob1.hashStruct()), "c52c0ee5d84264471806290a3f2c4cecfc5490626bf912d01f240d7a274b371e");

    EXPECT_EQ(hex(msgEIP712Domain.hashStruct()), "f2cee375fa42b42143804025fc449deafd50cc031ca257e0b194a650a912090f");

    Address address = Address(privateKeyCow.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    EXPECT_EQ(address.string(), "0xCD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826");

    // TODO sig
    // 0xbe609aee343fb3c4b28e1df9e632fca64fcfaede20f02e86244efddf30957bd2
    // 0x4355c47d63924e8a72e509b65029052eb6c299d53a04e167c5775fd466751c9d07299936d304c153f6443dfa05f40ff007d72911b6f72307f996231605b915621c
}

TEST(EthereumAbiStruct, encodeTypes_v3_Json) {
    auto hash = ParamStruct::hashStructJson("Mail",
        R"({
            "from": {"name": "Cow", "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"},
            "to": {"name": "Bob", "wallet": "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB"},
            "contents": "Hello, Bob!"
        })",
        R"([
            {"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]},
            {"Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person"}, {"name": "contents", "type": "string"}]}
        ])");
    ASSERT_EQ(hex(hash), "c52c0ee5d84264471806290a3f2c4cecfc5490626bf912d01f240d7a274b371e");
}

// See 'signedTypeData_v4' in https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts
TEST(EthereumAbiStruct, encodeTypes_v4) {
    EXPECT_EQ(msgGroup.encodeType(), "Group(string name,Person[] members)Person(string name,address[] wallets)");

    EXPECT_EQ(msgPersonCow2.encodeType(), "Person(string name,address[] wallets)");

    EXPECT_EQ(hex(msgPersonCow2.hashType()), "fabfe1ed996349fc6027709802be19d047da1aa5d6894ff5f6486d92db2e6860");

    EXPECT_EQ(hex(msgPersonCow2.encodeHashes()), 
        "fabfe1ed996349fc6027709802be19d047da1aa5d6894ff5f6486d92db2e6860"
        "8c1d2bd5348394761719da11ec67eedae9502d137e8940fee8ecd6f641ee1648"
        "8a8bfe642b9fc19c25ada5dadfd37487461dc81dd4b0778f262c163ed81b5e2a");

    EXPECT_EQ(hex(msgPersonCow2.hashStruct()), "9b4846dd48b866f0ac54d61b9b21a9e746f921cefa4ee94c4c0a1c49c774f67f");

    EXPECT_EQ(hex(msgPersonBob3.encodeHashes()), 
        "fabfe1ed996349fc6027709802be19d047da1aa5d6894ff5f6486d92db2e6860"
        "28cac318a86c8a0a6a9156c2dba2c8c2363677ba0514ef616592d81557e679b6"
        "d2734f4c86cc3bd9cabf04c3097589d3165d95e4648fc72d943ed161f651ec6d");

    EXPECT_EQ(hex(msgPersonBob3.hashStruct()), "efa62530c7ae3a290f8a13a5fc20450bdb3a6af19d9d9d2542b5a94e631a9168");

    EXPECT_EQ(msgMailCow2Bob3.encodeType(), "Mail(Person from,Person[] to,string contents)Person(string name,address[] wallets)");

    EXPECT_EQ(hex(msgMailCow2Bob3.hashType()), "4bd8a9a2b93427bb184aca81e24beb30ffa3c747e2a33d4225ec08bf12e2e753");

    EXPECT_EQ(hex(msgMailCow2Bob3.encodeHashes()), 
        "4bd8a9a2b93427bb184aca81e24beb30ffa3c747e2a33d4225ec08bf12e2e753"
        "9b4846dd48b866f0ac54d61b9b21a9e746f921cefa4ee94c4c0a1c49c774f67f"
        "ca322beec85be24e374d18d582a6f2997f75c54e7993ab5bc07404ce176ca7cd"
        "b5aadf3154a261abdd9086fc627b61efca26ae5702701d05cd2305f7c52a2fc8");

    EXPECT_EQ(hex(msgMailCow2Bob3.hashStruct()), "eb4221181ff3f1a83ea7313993ca9218496e424604ba9492bb4052c03d5c3df8");

    EXPECT_EQ(hex(msgEIP712Domain.hashStruct()), "f2cee375fa42b42143804025fc449deafd50cc031ca257e0b194a650a912090f");

    Address address = Address(privateKeyCow.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    EXPECT_EQ(address.string(), "0xCD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826");

    // TODO sig
    // 0xa85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2
    // 0x65cbd956f2fae28a601bebc9b906cea0191744bd4c4247bcd27cd08f8eb6b71c78efdf7a31dc9abee78f492292721f362d296cf86b4538e07b51303b67f749061b
}

TEST(EthereumAbiStruct, encodeTypes_v4_Json) {
    auto hash = ParamStruct::hashStructJson("Mail",
        R"({
            "from": {
                "name": "Cow",
                "wallets": [
                    "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
                    "DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
                ]
            },
            "to": [
                {
                    "name": "Bob",
                    "wallets": [
                        "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
                        "B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57",
                        "B0B0b0b0b0b0B000000000000000000000000000"
                    ]
                }
            ],
            "contents": "Hello, Bob!"
        })",
        R"([
            {"Person": [{"name": "name", "type": "string"}, {"name": "wallets", "type": "address[]"}]},
            {"Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person[]"}, {"name": "contents", "type": "string"}]}
        ])");
    ASSERT_EQ(hex(hash), "eb4221181ff3f1a83ea7313993ca9218496e424604ba9492bb4052c03d5c3df8");
}

// See 'signedTypeData_v4 with recursive types' in https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts
TEST(EthereumAbiStruct, encodeTypes_v4Rec) {
    ParamStruct msgPersonRecursiveMother("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Lyanna")),
        std::make_shared<ParamNamed>("mother", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{})),
        std::make_shared<ParamNamed>("father", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{
            std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Rickard")),
            std::make_shared<ParamNamed>("mother", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{})),
            std::make_shared<ParamNamed>("father", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{}))
        }))
    });
    ParamStruct msgPersonRecursiveFather("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Rhaegar")),
        std::make_shared<ParamNamed>("mother", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{})),
        std::make_shared<ParamNamed>("father", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{
            std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Aeris II")),
            std::make_shared<ParamNamed>("mother", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{})),
            std::make_shared<ParamNamed>("father", std::make_shared<ParamStruct>("Person", std::vector<std::shared_ptr<ParamNamed>>{}))
        }))
    });
    ParamStruct msgPersonRecursive("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Jon")),
        std::make_shared<ParamNamed>("mother", std::make_shared<ParamStruct>(msgPersonRecursiveMother)),
        std::make_shared<ParamNamed>("father", std::make_shared<ParamStruct>(msgPersonRecursiveFather))
    });

    EXPECT_EQ(msgPersonRecursive.encodeType(), "Person(string name,Person mother,Person father)");

    EXPECT_EQ(hex(msgPersonRecursive.hashType()), "7c5c8e90cb92c8da53b893b24962513be98afcf1b57b00327ae4cc14e3a64116");

    EXPECT_EQ(hex(msgPersonRecursiveMother.encodeHashes()), 
        "7c5c8e90cb92c8da53b893b24962513be98afcf1b57b00327ae4cc14e3a64116"
        "afe4142a2b3e7b0503b44951e6030e0e2c5000ef83c61857e2e6003e7aef8570"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "88f14be0dd46a8ec608ccbff6d3923a8b4e95cdfc9648f0db6d92a99a264cb36");

    EXPECT_EQ(hex(msgPersonRecursiveMother.hashStruct()), "9ebcfbf94f349de50bcb1e3aa4f1eb38824457c99914fefda27dcf9f99f6178b");

    EXPECT_EQ(hex(msgPersonRecursiveFather.encodeHashes()), 
        "7c5c8e90cb92c8da53b893b24962513be98afcf1b57b00327ae4cc14e3a64116"
        "b2a7c7faba769181e578a391a6a6811a3e84080c6a3770a0bf8a856dfa79d333"
        "0000000000000000000000000000000000000000000000000000000000000000"
        "02cc7460f2c9ff107904cff671ec6fee57ba3dd7decf999fe9fe056f3fd4d56e");

    EXPECT_EQ(hex(msgPersonRecursiveFather.hashStruct()), "b852e5abfeff916a30cb940c4e24c43cfb5aeb0fa8318bdb10dd2ed15c8c70d8");

    EXPECT_EQ(hex(msgPersonRecursive.encodeHashes()), 
        "7c5c8e90cb92c8da53b893b24962513be98afcf1b57b00327ae4cc14e3a64116"
        "e8d55aa98b6b411f04dbcf9b23f29247bb0e335a6bc5368220032fdcb9e5927f"
        "9ebcfbf94f349de50bcb1e3aa4f1eb38824457c99914fefda27dcf9f99f6178b"
        "b852e5abfeff916a30cb940c4e24c43cfb5aeb0fa8318bdb10dd2ed15c8c70d8");

    EXPECT_EQ(hex(msgPersonRecursive.hashStruct()), "fdc7b6d35bbd81f7fa78708604f57569a10edff2ca329c8011373f0667821a45");

    ParamStruct msgEIP712Domain("EIP712Domain", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Family Tree")),
        std::make_shared<ParamNamed>("version", std::make_shared<ParamString>("1")),
        std::make_shared<ParamNamed>("chainId", std::make_shared<ParamUInt256>(1)),
        std::make_shared<ParamNamed>("verifyingContract", std::make_shared<ParamAddress>(parse_hex("CcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC")))
    });

    EXPECT_EQ(hex(msgEIP712Domain.hashStruct()), "facb2c1888f63a780c84c216bd9a81b516fc501a19bae1fc81d82df590bbdc60");

    PrivateKey privateKeyDragon = PrivateKey(Hash::keccak256(TW::data("dragon")));
    Address address = Address(privateKeyDragon.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    EXPECT_EQ(address.string(), "0x065a687103C9F6467380beE800ecD70B17f6b72F");

    // TODO sig
    // 0x807773b9faa9879d4971b43856c4d60c2da15c6f8c062bd9d33afefb756de19c
    // 0xf2ec61e636ff7bb3ac8bc2a4cc2c8b8f635dd1b2ec8094c963128b358e79c85c5ca6dd637ed7e80f0436fe8fce39c0e5f2082c9517fe677cc2917dcd6c84ba881c
}

TEST(EthereumAbiStruct, encodeTypes_v4Rec_Json) {
    auto hash = ParamStruct::hashStructJson("Person",
        R"({
            "name": "Jon",
            "mother": {
                "name": "Lyanna",
                "father": {
                    "name": "Rickard"
                }
            },
            "father": {
                "name": "Rhaegar",
                "father": {
                    "name": "Aeris II"
                }
            }
        })",
        R"([
            {"Person": [
                {"name": "name", "type": "string"},
                {"name": "mother", "type": "Person"},
                {"name": "father", "type": "Person"}
            ]}
        ])");
    ASSERT_EQ(hex(hash), "fdc7b6d35bbd81f7fa78708604f57569a10edff2ca329c8011373f0667821a45");
}

// See 'signedTypeData' in https://github.com/MetaMask/eth-sig-util/blob/main/test/index.ts
TEST(EthereumAbiStruct, encodeTypeCow1) {
    ParamStruct msgPersonCow1("Person", std::vector<std::shared_ptr<ParamNamed>>{
        std::make_shared<ParamNamed>("name", std::make_shared<ParamString>("Cow")),
        std::make_shared<ParamNamed>("wallet", std::make_shared<ParamAddress>(parse_hex("CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826")))
    });

    EXPECT_EQ(msgPersonCow1.encodeType(), "Person(string name,address wallet)");

    EXPECT_EQ(hex(msgPersonCow1.hashType()), "b9d8c78acf9b987311de6c7b45bb6a9c8e1bf361fa7fd3467a2163f994c79500");

    EXPECT_EQ(hex(msgPersonCow1.encodeHashes()), "b9d8c78acf9b987311de6c7b45bb6a9c8e1bf361fa7fd3467a2163f994c795008c1d2bd5348394761719da11ec67eedae9502d137e8940fee8ecd6f641ee1648000000000000000000000000cd2a3d9f938e13cd947ec05abc7fe734df8dd826");

    EXPECT_EQ(hex(msgPersonCow1.hashStruct()), "fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8");
}

TEST(EthereumAbiStruct, hashStructJson) {
    {
        auto hash = ParamStruct::hashStructJson("Person",
            R"({"name": "Cow", "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"})",
            R"([{"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]}])");
        ASSERT_EQ(hex(hash), "fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8");
    }
}

TEST(EthereumAbiStruct, ParamFactoryMakeNamed) {
    std::shared_ptr<ParamNamed> p = ParamFactory::makeNamed("firstparam", "uint256");
    EXPECT_EQ(p->getName(), "firstparam");
    ASSERT_NE(p->getParam().get(), nullptr);
    EXPECT_EQ(p->getParam()->getType(), "uint256");
}

TEST(EthereumAbiStruct, ParamFactoryMakeStruct) {
    {
        std::shared_ptr<ParamStruct> s = ParamStruct::makeStruct("Person",
            R"(
                {"name": "Cow", "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"}
            )",
            R"(
                [{"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]}]
            )");
        ASSERT_NE(s.get(), nullptr);
        EXPECT_EQ(s->getType(), "Person");
        ASSERT_EQ(s->getCount(), 2);
        ASSERT_EQ(s->encodeType(), "Person(string name,address wallet)");
        ASSERT_EQ(hex(s->hashStruct()), "fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8");
    }
}

#define EXPECT_EXCEPTION(statement, exceptionMsg) \
    try { \
        statement; \
        FAIL() << "No exception"; \
    } catch (const std::invalid_argument& ex) { \
        EXPECT_EQ(std::string(ex.what()), exceptionMsg); \
    } catch (...) { \
        FAIL() << "Not the expected exception"; \
    }

TEST(EthereumAbiStruct, ParamFactoryMakeTypes) {
    {
        std::vector<std::shared_ptr<ParamStruct>> tt = ParamStruct::makeTypes(R"([{"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]}])");
        ASSERT_EQ(tt.size(), 1);
        EXPECT_EQ(tt[0]->encodeType(), "Person(string name,address wallet)");
    }
    {
        std::vector<std::shared_ptr<ParamStruct>> tt = ParamStruct::makeTypes(
            R"([
                {"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]},
                {"Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person"}, {"name": "contents", "type": "string"}]}
            ])");
        ASSERT_EQ(tt.size(), 2);
        EXPECT_EQ(tt[0]->encodeType(), "Person(string name,address wallet)");
        EXPECT_EQ(tt[1]->encodeType(), "Mail(Person from,Person to,string contents)Person(string name,address wallet)");
    }
    {   // edge cases
        EXPECT_EXCEPTION(ParamStruct::makeTypes("NOT_A_JSON"), "Could not parse types Json");
        EXPECT_EXCEPTION(ParamStruct::makeTypes("+/{\\"), "Could not parse types Json");
        EXPECT_EXCEPTION(ParamStruct::makeTypes(""), "Could not parse types Json");
        EXPECT_EXCEPTION(ParamStruct::makeTypes("0"), "Expecting array");
        EXPECT_EXCEPTION(ParamStruct::makeTypes("{\"a\": 0}"), "Expecting array");
        EXPECT_EQ(ParamStruct::makeTypes("[]").size(), 0);
        EXPECT_EXCEPTION(ParamStruct::makeTypes("[{}]"), "No valid params found");
        EXPECT_EXCEPTION(ParamStruct::makeTypes(R"([{"name": 0}])"), "Expecting array, name");
        // wrong order, references type comes later
        EXPECT_EXCEPTION(ParamStruct::makeTypes(R"([{"Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person"}, {"name": "contents", "type": "string"}]}, {"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]}])"), "Unknown type Person");
    }
}

TEST(EthereumAbiStruct, ParamFactoryMakeType) {
    {
        std::shared_ptr<ParamStruct> t = ParamStruct::makeType(R"({"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}]})");
        EXPECT_NE(t.get(), nullptr);
        EXPECT_EQ(t->getType(), "Person");
        ASSERT_EQ(t->getCount(), 2);
        ASSERT_EQ(t->encodeType(), "Person(string name,address wallet)");
    }
    {   // edge cases
        EXPECT_EXCEPTION(ParamStruct::makeType("NOT_A_JSON"), "Could not parse type Json");
        EXPECT_EXCEPTION(ParamStruct::makeType("+/{\\"), "Could not parse type Json");
        EXPECT_EXCEPTION(ParamStruct::makeType(""), "Could not parse type Json");
        EXPECT_EXCEPTION(ParamStruct::makeType("{}"), "No valid params found");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": 0})"), "Expecting array, Person");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": []})"), "No valid params found");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": [{"dummy": 0}]})"), "Could not process Json");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": [{"name": "val"}]})"), "Could not process Json");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": [{"type": "val"}]})"), "Could not process Json");
        EXPECT_EXCEPTION(ParamStruct::makeType(R"({"Person": [{"name": "name", "type": "INVALID_TYPE"}, {"name": "wallet", "type": "address"}]})"), "Unknown type INVALID_TYPE");
    }
}
