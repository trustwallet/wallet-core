// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/AddressV3.h"

#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::Cardano;
using namespace std;


TEST(CardanoAddress, Validation) {
    // valid V3 address
    ASSERT_TRUE(AddressV3::isValid("addr1s3hdtrqgs47l7ue5srga8wmk9dzw279x9e7lxadalt6z0fk64nnn2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59j5lempe"));
    ASSERT_TRUE(AddressV3::isValid("addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j"));
    ASSERT_TRUE(AddressV3::isValid("ca1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s"));
    ASSERT_TRUE(AddressV3::isValid("ca1qsqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jq2f29vkz6t30xqcnyve5x5mrwwpe8ganc0f78aqyzsjrg3z5v36gguhxny"));

    // valid V2 address
    ASSERT_TRUE(AddressV3::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx"));
    ASSERT_TRUE(AddressV3::isValid("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W"));

    // valid V1 address
    ASSERT_TRUE(AddressV3::isValid("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ"));
    ASSERT_TRUE(AddressV3::isValid("DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di"));

    // invalid checksum V3
    ASSERT_FALSE(AddressV3::isValid("PREFIX1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s"));
    // invalid checksum V2
    ASSERT_FALSE(AddressV3::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm"));
    // random
    ASSERT_FALSE(AddressV3::isValid("hasoiusaodiuhsaijnnsajnsaiussai"));
    // empty
    ASSERT_FALSE(AddressV3::isValid(""));
}

TEST(CardanoAddress, FromStringV2) {
    {
        auto address = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    }
    {
        auto address = AddressV3("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
        ASSERT_EQ(address.string(), "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
    }
}

TEST(CardanoAddress, FromStringV3) {
    {
        // single addr
        auto address = AddressV3("ca1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s");
        EXPECT_EQ(address.string("ca"), "ca1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s");
        EXPECT_EQ(address.string("ca"), "ca1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s");
        EXPECT_EQ(AddressV3::Discrim_Production, address.discrimination);
        EXPECT_EQ(AddressV3::Kind_Single, address.kind);
        EXPECT_EQ("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20", hex(address.key1));
        EXPECT_EQ("", hex(address.groupKey));
    }
    {
        // group addr
        auto address = AddressV3("addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j");
        EXPECT_EQ(address.string(), "addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j");
        EXPECT_EQ(AddressV3::Discrim_Test, address.discrimination);
        EXPECT_EQ(AddressV3::Kind_Group, address.kind);
        EXPECT_EQ("4dc3393959139f00bc88d33d4b2582fecc237238f2269fc094b286acdb892295", hex(address.key1));
        EXPECT_EQ("6d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2", hex(address.groupKey));
    }
}

TEST(CardanoAddress, MnemonicToAddressV2) {
    {
        // Test from cardano-crypto.js; Test wallet
        auto mnemonic = "cost dash dress stove morning robust group affair stomach vacant route volume yellow salute laugh";
        auto wallet = HDWallet(mnemonic, "");

        PrivateKey masterPrivKey = wallet.getMasterKey(TWCurve::TWCurveED25519Extended);
        PrivateKey masterPrivKeyExt = wallet.getMasterKeyExtension(TWCurve::TWCurveED25519Extended);
        ASSERT_EQ("a018cd746e128a0be0782b228c275473205445c33b9000a33dd5668b430b5744", hex(masterPrivKey.bytes));
        ASSERT_EQ("26877cfe435fddda02409b839b7386f3738f10a30b95a225f4b720ee71d2505b", hex(masterPrivKeyExt.bytes));

        {
            string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
            EXPECT_EQ("addr1sna05l45z33zpkm8z44q8f0h57wxvm0c86e34wlmua7gtcrdgrdrzy8ny3walyfjanhe33nsyuh088qr5gepqaen6jsa9r94xvvd7fh6jc3e6x", addr);
        }
        {
            PrivateKey privKey0 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/0"));
            PublicKey pubKey0 = privKey0.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr0 = AddressV2(pubKey0);
            EXPECT_EQ("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W", addr0.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/1"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr1 = AddressV2(pubKey1);
            EXPECT_EQ("Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV", addr1.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/2"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr1 = AddressV2(pubKey1);
            EXPECT_EQ("Ae2tdPwUPEZ8LAVy21zj4BF97iWxKCmPv12W6a18zLX3V7rZDFFVgqUBkKw", addr1.string());
        }
        {
            PrivateKey privKey0 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/0"));
            PublicKey pubKey0 = privKey0.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr0 = AddressV3(pubKey0);
            EXPECT_EQ("addr1sna05l45z33zpkm8z44q8f0h57wxvm0c86e34wlmua7gtcrdgrdrzy8ny3walyfjanhe33nsyuh088qr5gepqaen6jsa9r94xvvd7fh6jc3e6x", addr0.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/1"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr1 = AddressV3(pubKey1);
            EXPECT_EQ("addr1sjkw630aatyg273m9cpgezvs2unf6xrtw0z7udhguh7ednkhf9p0jduldrg5qsnaz99e3sl4f8t56w0hs0zhql9lacr63mx693ppjw2r5nwehs", addr1.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/2"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            auto addr1 = AddressV3(pubKey1);
            EXPECT_EQ("addr1sng939f9el5mnsj4l30kk2f02ea63rwhny5pa69masam4xtsmp5naq6lks0p7pzkn35z7juyd7hhk3zc8p9dc736pu4nzhyy6fusxapa9v5h5c", addr1.string());
        }
    }
    {
        auto mnemonicPlay1 = "youth away raise north opinion slice dash bus soldier dizzy bitter increase saddle live champion";
        auto wallet = HDWallet(mnemonicPlay1, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1ssf3e4w2g8gpqlewnt0a4t9kwvdwhxyaaqu7tmru20zgakwf2mdu3jamu779gr3085lykk7r0q8t6lf6p2vfj7u9ma2s7a748vn0se2gxym6da", addr);
    }
    {
        auto mnemonicPlay2 = "return custom two home gain guilt kangaroo supply market current curtain tomorrow heavy blue robot";
        auto wallet = HDWallet(mnemonicPlay2, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1sn8hmvmhxw6926mgz3fn5qp205wmu42adg8uehnce3nfr4umecm3mfqmxy4jyl5xewag3kq52vulqpgt386atv3v5upz9j0rl4cc42m707gewk", addr);
    }
    {
        auto mnemonicALDemo = "civil void tool perfect avocado sweet immense fluid arrow aerobic boil flash";
        auto wallet = HDWallet(mnemonicALDemo, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1sn32yvavqtnzpqggxf9aa0yypng80gr3anfpwppz8dhztx4cevzp5v6nf40c6d8v6z70fcd76634sdlyfpfpk6d3ya84czk83jlze676vmpf37", addr);
    }
    {
        // V2 Tested against AdaLite
        auto mnemonicPlay1 = "youth away raise north opinion slice dash bus soldier dizzy bitter increase saddle live champion";
        auto wallet = HDWallet(mnemonicPlay1, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h", addr);
    }
    {
        // V2 Tested against AdaLite
        auto mnemonicPlay2 = "return custom two home gain guilt kangaroo supply market current curtain tomorrow heavy blue robot";
        auto wallet = HDWallet(mnemonicPlay2, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZLtJx7LA2XZ3zzwonH9x9ieX3dMzaTBD3TfXuKczjMSjTecr1", addr);
    }
    {
        // V2 AdaLite Demo phrase, 12-word.  AdaLite uses V1 for it, in V2 it produces different addresses.
        // In AdaLite V1 addr0 is DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di
        auto mnemonicALDemo = "civil void tool perfect avocado sweet immense fluid arrow aerobic boil flash";
        auto wallet = HDWallet(mnemonicALDemo, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZJbLcD8iLgN7hVGvq66WdR4zocntRekSP97Ds3MvCfmEDjJYu", addr);
    }
}

TEST(CardanoAddress, KeyHashV2) {
    auto xpub = parse_hex("e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000");
    auto hash = AddressV2::keyHash(xpub);
    ASSERT_EQ("a1eda96a9952a56c983d9f49117f935af325e8a6c9d38496e945faa8", hex(hash));
}

TEST(CardanoAddress, FromPublicKeyInternalV3) {
    // tests from chain-lib
    {
        auto address = AddressV3::createSingle(AddressV3::Discrim_Production, parse_hex("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20"));
        auto bech = address.string("ca");
        EXPECT_EQ("ca1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s", bech);
        EXPECT_EQ("amaqeayeaudaocajbifqydiob4ibceqtcqkrmfyydenbwha5dypsa", address.stringBase32());
    }
    {
        auto address = AddressV3::createGroup(AddressV3::Discrim_Production, 
            parse_hex("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20"),
            parse_hex("292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748")
        );
        auto bech = address.string("ca");
        EXPECT_EQ("ca1qsqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jq2f29vkz6t30xqcnyve5x5mrwwpe8ganc0f78aqyzsjrg3z5v36gguhxny", bech);        
        EXPECT_EQ("aqaqeayeaudaocajbifqydiob4ibceqtcqkrmfyydenbwha5dypsakjkfmwc2lrpgaytemzugu3doobzhi5typj6h5aecqsdircumr2i", address.stringBase32());
    }
    {
        auto address = AddressV3::createGroup(AddressV3::Discrim_Test, 
            parse_hex("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f20"),
            parse_hex("292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748")
        );
        auto bech = address.string("ca");
        EXPECT_EQ("ca1ssqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jq2f29vkz6t30xqcnyve5x5mrwwpe8ganc0f78aqyzsjrg3z5v36gjdetkp", bech);        
        EXPECT_EQ("qqaqeayeaudaocajbifqydiob4ibceqtcqkrmfyydenbwha5dypsakjkfmwc2lrpgaytemzugu3doobzhi5typj6h5aecqsdircumr2i", address.stringBase32());
    }
    {
        auto address = AddressV3::createAccount(AddressV3::Discrim_Test, parse_hex("292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748"));
        auto bech = address.string("ca");
        EXPECT_EQ("ca1s55j52ev95hz7vp3xgengdfkxuurjw3m8s7nu06qg9pyx3z9ger5samu4rv", bech);
        EXPECT_EQ("quusukzmfuxc6mbrgiztinjwg44dsor3hq6t4p2aifbegrcfizduq", address.stringBase32());
    }
    // end of chain-lib
    {
        auto address = AddressV3::createGroup(AddressV3::Discrim_Test, 
            parse_hex("4dc3393959139f00bc88d33d4b2582fecc237238f2269fc094b286acdb892295"),
            parse_hex("6d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2")
        );
        auto bech = address.string("addr");
        EXPECT_EQ("addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j", bech);        
    }
}

TEST(CardanoAddress, FromPublicKeyV2) {
    {
        // caradano-crypto.js test
        auto publicKey = PublicKey(parse_hex("e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"), TWPublicKeyTypeED25519Extended);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZCxt4UV1Uj2AMMRvg5pYPypqZowVptz3GYpK4pkcvn3EjkuNH");
    }
    {
        // Adalite test account addr0
        auto publicKey = PublicKey(parse_hex("57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"), TWPublicKeyTypeED25519Extended);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");
    }
    {
        // Adalite test account addr1
        auto publicKey = PublicKey(parse_hex("25af99056d600f7956312406bdd1cd791975bb1ae91c9d034fc65f326195fcdb247ee97ec351c0820dd12de4ca500232f73a35fe6f86778745bcd57f34d1048d"), TWPublicKeyTypeED25519Extended);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV");
    }
    {
        // Play1 addr0
        auto publicKey = PublicKey(parse_hex("7cee0f30b9d536a786547dd77b35679b6830e945ffde768eb4f2a061b9dba016e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2"), TWPublicKeyTypeED25519Extended);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h");
    }
}

TEST(CardanoAddress, FromPrivateKeyV2) {
    {
        // mnemonic Test, addr0
        auto privateKey = PrivateKey(
            parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
            parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        ASSERT_EQ(hex(publicKey.bytes), "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");
    }
    {
        // mnemonic Play1, addr0
        auto privateKey = PrivateKey(
            parse_hex("a089c9423100960440ccd5b7adbd202d1ab1993a7bb30fc88b287d94016df247"),
            parse_hex("da86a87f08fb15de1431a6c0ccd5ebf51c3bee81f7eaf714801bbbe4d903154a"),
            parse_hex("e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2")
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        ASSERT_EQ(hex(publicKey.bytes), "7cee0f30b9d536a786547dd77b35679b6830e945ffde768eb4f2a061b9dba016e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h");
    }
    {
        // from cardano-crypto.js test
        auto privateKey = PrivateKey(
            parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
            parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
            parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000")
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        ASSERT_EQ(hex(publicKey.bytes), "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZCxt4UV1Uj2AMMRvg5pYPypqZowVptz3GYpK4pkcvn3EjkuNH");
    }
}

TEST(CardanoAddress, PrivateKeyExtended) {
    // check extended key lengths, private key 3x32 bytes, public key 64 bytes
    auto privateKeyExt = PrivateKey(
        parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
        parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
        parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
    );
    auto publicKeyExt = privateKeyExt.getPublicKey(TWPublicKeyTypeED25519Extended);
    ASSERT_EQ(64, publicKeyExt.bytes.size());

    // Non-extended: both are 32 bytes.
    auto privateKeyNonext = PrivateKey(
        parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744")
    );
    auto publicKeyNonext = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(32, publicKeyNonext.bytes.size());
}

TEST(CardanoAddress, FromStringNegativeInvalidString) {
    try {
        auto address = AddressV3("__INVALID_ADDRESS__");
    } catch (...) {
        return;
    }
    FAIL() << "Expected exception!";
}

TEST(CardanoAddress, FromStringNegativeBadChecksumV2) {
    try {
        auto address = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm");
    } catch (...) {
        return;
    }
    FAIL() << "Expected exception!";
}

TEST(CardanoAddress, DataV3) {
    // group addr
    auto address = AddressV3("addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j");
    EXPECT_EQ("4dc3393959139f00bc88d33d4b2582fecc237238f2269fc094b286acdb892295", hex(address.key1));
    EXPECT_EQ("6d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2", hex(address.groupKey));
    Data data = address.data();
    EXPECT_EQ(
        "0104"
        "20" "4dc3393959139f00bc88d33d4b2582fecc237238f2269fc094b286acdb892295"
        "20" "6d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2",
        hex(data)
    );
}

TEST(CardanoAddress, FromDataV3) {
    Data data = parse_hex("0104204dc3393959139f00bc88d33d4b2582fecc237238f2269fc094b286acdb892295206d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2");
    auto address = AddressV3(data);
    EXPECT_EQ(address.string(), "addr1s3xuxwfetyfe7q9u3rfn6je9stlvcgmj8rezd87qjjegdtxm3y3f2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59jz29g8j");
    EXPECT_EQ("6d0b99fdb1cc851fbc8dbcb7d6710fde28df46360c259d482db679c24c4e50b2", hex(address.groupKey));
}

TEST(CardanoAddress, CopyConstructorLegacy) {
    AddressV3 address1 = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    EXPECT_TRUE(address1.legacyAddressV2.has_value());
    AddressV3 address2 = AddressV3(address1);
    EXPECT_TRUE(address2.legacyAddressV2.has_value());
    EXPECT_TRUE(*(address2.legacyAddressV2) == *(address1.legacyAddressV2));
    // if it was not a deep copy, double freeing would occur
}

TEST(CardanoAddress, AssignmentOperatorLegacy) {
    AddressV3 addr1leg = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    EXPECT_TRUE(addr1leg.legacyAddressV2.has_value());
    AddressV3 addr2nonleg = AddressV3("addr1s3hdtrqgs47l7ue5srga8wmk9dzw279x9e7lxadalt6z0fk64nnn2mgtn87mrny9r77gm09h6ecslh3gmarrvrp9n4yzmdnecfxyu59j5lempe");
    EXPECT_FALSE(addr2nonleg.legacyAddressV2.has_value());
    AddressV3 addr3leg = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    EXPECT_TRUE(addr3leg.legacyAddressV2.has_value());

    AddressV3 address = addr1leg;
    EXPECT_TRUE(address.legacyAddressV2.has_value());
    EXPECT_TRUE(*address.legacyAddressV2 == *addr1leg.legacyAddressV2);
    address = addr2nonleg;
    EXPECT_FALSE(address.legacyAddressV2.has_value());
    address = addr3leg;
    EXPECT_TRUE(address.legacyAddressV2.has_value());
    EXPECT_TRUE(*address.legacyAddressV2 == *addr3leg.legacyAddressV2);
}
