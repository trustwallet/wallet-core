// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/AddressV3.h"

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace std;

namespace TW::Cardano::tests {

const auto dummyKey = parse_hex("1111111111111111111111111111111111111111111111111111111111111111");

TEST(CardanoAddress, V3NetworkIdKind) {
    EXPECT_EQ(AddressV3::firstByte(AddressV3::Network_Test, AddressV3::Kind_Base), 0);
    EXPECT_EQ(AddressV3::firstByte(AddressV3::Network_Production, AddressV3::Kind_Base), 1);
    EXPECT_EQ(AddressV3::firstByte(AddressV3::NetworkId(2), AddressV3::Kind(3)), 50);

    EXPECT_EQ(AddressV3::networkIdFromFirstByte(0), AddressV3::Network_Test);
    EXPECT_EQ(AddressV3::networkIdFromFirstByte(1), AddressV3::Network_Production);
    EXPECT_EQ(AddressV3::networkIdFromFirstByte(50), AddressV3::NetworkId(2));

    EXPECT_EQ(AddressV3::kindFromFirstByte(0), AddressV3::Kind_Base);
    EXPECT_EQ(AddressV3::kindFromFirstByte(1), AddressV3::Kind_Base);
    EXPECT_EQ(AddressV3::kindFromFirstByte(50), AddressV3::Kind(3));
}

TEST(CardanoAddress, Validation) {
    // valid V3 address
    ASSERT_TRUE(AddressV3::isValidLegacy("addr1v9wa6entm75duchtu50mu6u6hkagdgqzaevt0cwryaw3pnca870vt"));
    ASSERT_TRUE(AddressV3::isValidLegacy("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"));

    ASSERT_TRUE(AddressV3::isValid("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23"));
    ASSERT_TRUE(AddressV3::isValid("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5"));
    ASSERT_TRUE(AddressV3::isValid("addr1vyuca7esanpgs4ke0um3ft6f4yaeuz3ftpfqx9nxpct2uyqu7dvlp")); // enterprise
    ASSERT_TRUE(AddressV3::isValid("stake1uy9ggsc9qls4pu9qvyyacwnmr9tt0gzcdt5s0zj4au8qkqc65geks")); // reward
    ASSERT_TRUE(AddressV3::isValid("addr1sxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qmxapsy")); // kind 8

    // valid V2 address
    ASSERT_TRUE(AddressV3::isValidLegacy("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx"));
    ASSERT_TRUE(AddressV3::isValidLegacy("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W"));

    ASSERT_FALSE(AddressV3::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx"));

    // valid V1 address
    ASSERT_TRUE(AddressV3::isValidLegacy("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ"));
    ASSERT_TRUE(AddressV3::isValidLegacy("DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di"));

    // invalid V3, invalid network
    ASSERT_FALSE(AddressV3::isValidLegacy("addr1sna05l45z33zpkm8z44q8f0h57wxvm0c86e34wlmua7gtcrdgrdrzy8ny3walyfjanhe33nsyuh088qr5gepqaen6jsa9r94xvvd7fh6jc3e6x"));
    // invalid V3, invalid prefix
    ASSERT_FALSE(AddressV3::isValidLegacy("prefix1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35q3hm7lv"));
    // invalid V3, length
    ASSERT_FALSE(AddressV3::isValidLegacy("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32xsmpqws7"));
    // invalid checksum V3
    ASSERT_FALSE(AddressV3::isValidLegacy("PREFIX1qvqsyqcyq5rqwzqfpg9scrgwpugpzysnzs23v9ccrydpk8qarc0jqxuzx4s"));
    // invalid checksum V2
    ASSERT_FALSE(AddressV3::isValidLegacy("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm"));
    // invalid V2 address
    ASSERT_FALSE(AddressV2::isValid("73Fig6QU8N"));
    // random
    ASSERT_FALSE(AddressV3::isValidLegacy("hasoiusaodiuhsaijnnsajnsaiussai"));
    // empty
    ASSERT_FALSE(AddressV3::isValidLegacy(""));
    ASSERT_FALSE(AddressV3::isValidLegacy("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk2"));
}

TEST(CardanoAddress, FromStringV2) {
    {
        auto address = AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(address.kind, AddressV3::Kind_Base);
        EXPECT_EQ(hex(address.data()), "01" "8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034e" "cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
    {
        auto address = AddressV3("addr1qxteqxsgxrs4he9d28lh70qu7qfz7saj6dmxwsqyle2yp3xvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35quehtx3");
        EXPECT_EQ(address.kind, AddressV3::Kind_Base);
        EXPECT_EQ(hex(address.data()), "01" "97901a0830e15be4ad51ff7f3c1cf0122f43b2d376674004fe5440c4" "cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
    {
        auto address = AddressV3("addr1q8sfzcwce0fqll3symd7f0amayxqq68nxt2u8pgen9y00tkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35q40ytea");
        EXPECT_EQ(address.kind, AddressV3::Kind_Base);
        EXPECT_EQ(hex(address.data()), "01" "e09161d8cbd20ffe3026dbe4bfbbe90c0068f332d5c385199948f7ae" "cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
    {
        auto address = AddressV3("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    }
    {
        auto address = AddressV3("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
        ASSERT_EQ(address.string(), "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
    }
    {
        EXPECT_ANY_THROW(new AddressV3(""));
    }
}

TEST(CardanoAddress, FromStringV3_Base) {
    {
        auto address = AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(address.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(address.string("addr"), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(AddressV3::Network_Production, address.networkId);
        EXPECT_EQ(AddressV3::Kind_Base, address.kind);
        EXPECT_EQ("8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468", hex(address.bytes));
    }
}

TEST(CardanoAddress, FromStringV3_Enterprise) {
    {
        auto address = AddressV3("addr1vyuca7esanpgs4ke0um3ft6f4yaeuz3ftpfqx9nxpct2uyqu7dvlp");
        EXPECT_EQ(address.string(), "addr1vyuca7esanpgs4ke0um3ft6f4yaeuz3ftpfqx9nxpct2uyqu7dvlp");
        EXPECT_EQ(AddressV3::Network_Production, address.networkId);
        EXPECT_EQ(AddressV3::Kind_Enterprise, address.kind);
        EXPECT_EQ("398efb30ecc28856d97f3714af49a93b9e0a2958520316660e16ae10", hex(address.bytes));
    }
}

TEST(CardanoAddress, FromStringV3_Reward) {
    {
        auto address = AddressV3("stake1uy9ggsc9qls4pu9qvyyacwnmr9tt0gzcdt5s0zj4au8qkqc65geks");
        EXPECT_EQ(address.string(), "stake1uy9ggsc9qls4pu9qvyyacwnmr9tt0gzcdt5s0zj4au8qkqc65geks");
        EXPECT_EQ(AddressV3::Network_Production, address.networkId);
        EXPECT_EQ(AddressV3::Kind_Reward, address.kind);
        EXPECT_EQ("0a84430507e150f0a06109dc3a7b1956b7a0586ae9078a55ef0e0b03", hex(address.bytes));
    }
}

TEST(CardanoAddress, MnemonicToAddressV3) {
    {
        // Test from cardano-crypto.js; Test wallet
        const auto mnemonic = "cost dash dress stove morning robust group affair stomach vacant route volume yellow salute laugh";
        const auto coin = TWCoinTypeCardano;
        const auto derivPath = derivationPath(coin);

        const auto wallet = HDWallet(mnemonic, "");

        // check entropy
        EXPECT_EQ("30a6f50aeb58ff7699b822d63e0ef27aeff17d9f", hex(wallet.getEntropy()));

        {
            PrivateKey masterPrivKey = wallet.getMasterKey(TWCurve::TWCurveED25519ExtendedCardano);
            PrivateKey masterPrivKeyExt = wallet.getMasterKeyExtension(TWCurve::TWCurveED25519ExtendedCardano);
            // the two together matches first half of keypair
            ASSERT_EQ("a018cd746e128a0be0782b228c275473205445c33b9000a33dd5668b430b5744", hex(masterPrivKey.bytes));
            ASSERT_EQ("26877cfe435fddda02409b839b7386f3738f10a30b95a225f4b720ee71d2505b", hex(masterPrivKeyExt.bytes));

            PublicKey masterPublicKey = masterPrivKey.getPublicKey(TWPublicKeyTypeED25519);
            ASSERT_EQ("3aecb95953edd0b16db20366097ddedcb3512fe36193473c5fca2af774d44739", hex(masterPublicKey.bytes));
        }
        {
            string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
            EXPECT_EQ("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq", addr);
        }
        {
            auto addressData = addressToData(TWCoinType::TWCoinTypeCardano, "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
            EXPECT_EQ(hex(addressData), "01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b");
        }
        {
            const auto privateKey = wallet.getKey(coin, derivPath);
            EXPECT_EQ(hex(privateKey.bytes), "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fae0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276");
            const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);

            const auto address = AddressV3(publicKey);
            EXPECT_EQ(address.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
            EXPECT_EQ(address.networkId, AddressV3::Network_Production);
            EXPECT_EQ(address.kind, AddressV3::Kind_Base);
            EXPECT_EQ(hex(address.bytes), "8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034e"
                                          "cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
        }
        {
            PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/0"));
            EXPECT_EQ("e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b5744", hex(privateKey.key()));
            EXPECT_EQ("37aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f31", hex(privateKey.extension()));
            EXPECT_EQ("10f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fa", hex(privateKey.chainCode()));
            EXPECT_EQ("e0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744", hex(privateKey.secondKey()));
            EXPECT_EQ("424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5", hex(privateKey.secondExtension()));
            EXPECT_EQ("bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276", hex(privateKey.secondChainCode()));
            PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
            EXPECT_EQ("fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26faf4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276", hex(publicKey.bytes));
            string addr = AddressV3(publicKey).string();
            EXPECT_EQ("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq", addr);
        }
        {
            PrivateKey privKey0 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/0"));
            PublicKey pubKey0 = privKey0.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr0 = AddressV2(pubKey0);
            EXPECT_EQ("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W", addr0.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/1"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr1 = AddressV2(pubKey1);
            EXPECT_EQ("Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV", addr1.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/44'/1815'/0'/0/2"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr1 = AddressV2(pubKey1);
            EXPECT_EQ("Ae2tdPwUPEZ8LAVy21zj4BF97iWxKCmPv12W6a18zLX3V7rZDFFVgqUBkKw", addr1.string());
        }
        {
            PrivateKey privKey0 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/0"));
            PublicKey pubKey0 = privKey0.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr0 = AddressV3(pubKey0);
            EXPECT_EQ("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq", addr0.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/1"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr1 = AddressV3(pubKey1);
            EXPECT_EQ("addr1q9068st87h22h3l6w6t5evnlm067rag94llqya2hkjrsd3wvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qpmxzjt", addr1.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(TWCoinTypeCardano, DerivationPath("m/1852'/1815'/0'/0/2"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Cardano);
            auto addr1 = AddressV3(pubKey1);
            EXPECT_EQ("addr1qxteqxsgxrs4he9d28lh70qu7qfz7saj6dmxwsqyle2yp3xvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35quehtx3", addr1.string());
        }
    }
    {
        auto mnemonicPlay1 = "youth away raise north opinion slice dash bus soldier dizzy bitter increase saddle live champion";
        auto wallet = HDWallet(mnemonicPlay1, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1q83nm9ntq3eaz8dya49txxtle6nn8geq4gmyylrzhzs7v0qjdwm6zuahwwds6c7mj8t6a09rup6m2cnh6zvzddnafp2slmcu95", addr);
    }
    {
        auto mnemonicPlay2 = "return custom two home gain guilt kangaroo supply market current curtain tomorrow heavy blue robot";
        auto wallet = HDWallet(mnemonicPlay2, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1qywxuqm7dx0yvqnn2yllye9urz5f2e4fgwanluzh008r22e53hart525dxgjcl0xzm0kes4n5tan8f5pz7ej0tkzgyrqtfmlal", addr);
    }
    {
        auto mnemonicALDemo = "civil void tool perfect avocado sweet immense fluid arrow aerobic boil flash";
        auto wallet = HDWallet(mnemonicALDemo, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        EXPECT_EQ("addr1q94zzrtl32tjp8j96auatnhxd2y35fnk6wuxqvqm9364vp9spdkjdsmyfhvfagjzh4uzp9zs6p5djw89jac2g0ujs2eqsuy7pu", addr);
    }
    {
        // V2 Tested against AdaLite
        auto mnemonicPlay1 = "youth away raise north opinion slice dash bus soldier dizzy bitter increase saddle live champion";
        auto wallet = HDWallet(mnemonicPlay1, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h", addr);
    }
    {
        // V2 Tested against AdaLite
        auto mnemonicPlay2 = "return custom two home gain guilt kangaroo supply market current curtain tomorrow heavy blue robot";
        auto wallet = HDWallet(mnemonicPlay2, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZLtJx7LA2XZ3zzwonH9x9ieX3dMzaTBD3TfXuKczjMSjTecr1", addr);
    }
    {
        // V2 AdaLite Demo phrase, 12-word.  AdaLite uses V1 for it, in V2 it produces different addresses.
        // In AdaLite V1 addr0 is DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di
        auto mnemonicALDemo = "civil void tool perfect avocado sweet immense fluid arrow aerobic boil flash";
        auto wallet = HDWallet(mnemonicALDemo, "");
        PrivateKey privateKey = wallet.getKey(TWCoinTypeCardano, DerivationPath(TWPurposeBIP44, TWCoinTypeCardano, DerivationPathIndex(0, true).derivationIndex(), 0, 0));
        PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        string addr = AddressV2(publicKey).string();
        EXPECT_EQ("Ae2tdPwUPEZJbLcD8iLgN7hVGvq66WdR4zocntRekSP97Ds3MvCfmEDjJYu", addr);
    }
}

TEST(CardanoAddress, KeyHashV2) {
    auto xpub = parse_hex("e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000");
    auto hash = AddressV2::keyHash(xpub);
    ASSERT_EQ("a1eda96a9952a56c983d9f49117f935af325e8a6c9d38496e945faa8", hex(hash));
}

TEST(CardanoAddress, FromDataV3_Base) {
    auto address0 = AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    EXPECT_EQ(address0.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    EXPECT_EQ(hex(address0.data()), "018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    {
        auto address = AddressV3(parse_hex("018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468"));
        EXPECT_EQ(address.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(address.kind, AddressV3::Kind_Base);
        EXPECT_EQ(address.networkId, AddressV3::Network_Production);
        EXPECT_EQ(hex(address.bytes), "8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
    {
        auto address = AddressV3::createBase(AddressV3::Network_Production,
                                             PublicKey(parse_hex("fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da31"), TWPublicKeyTypeED25519),
                                             PublicKey(parse_hex("f4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4"), TWPublicKeyTypeED25519));
        EXPECT_EQ(address.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    }
    {
        EXPECT_ANY_THROW(AddressV3::createBase(AddressV3::Network_Production,
                                               PublicKey(parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be"), TWPublicKeyTypeSECP256k1),
                                               PublicKey(parse_hex("f4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4"), TWPublicKeyTypeED25519)));
    }
    {
        EXPECT_ANY_THROW(AddressV3::createBase(AddressV3::Network_Production,
                                               PublicKey(parse_hex("fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da31"), TWPublicKeyTypeED25519),
                                               PublicKey(parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be"), TWPublicKeyTypeSECP256k1)));
    }
    {
        EXPECT_ANY_THROW(AddressV3::createBase(AddressV3::Network_Production,
                                               parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be"),
                                               parse_hex("f4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4")));
    }
    {
        EXPECT_ANY_THROW(AddressV3::createBase(AddressV3::Network_Production,
                                               parse_hex("fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da31"),
                                               parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be")));
    }
}

TEST(CardanoAddress, FromPrivateKeyV3) {
    {
        // from cardano-crypto.js test
        auto privateKey = PrivateKey(
            parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
            parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
            parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"),
            dummyKey, dummyKey, dummyKey);
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        EXPECT_ANY_THROW(new AddressV3(publicKey));
    }
}

TEST(CardanoAddress, FromDataV3_Enterprise) {
    auto address = AddressV3(parse_hex("61398efb30ecc28856d97f3714af49a93b9e0a2958520316660e16ae10"));
    EXPECT_EQ(address.string(), "addr1vyuca7esanpgs4ke0um3ft6f4yaeuz3ftpfqx9nxpct2uyqu7dvlp");
    EXPECT_EQ(address.kind, AddressV3::Kind_Enterprise);
    EXPECT_EQ(address.networkId, AddressV3::Network_Production);
    EXPECT_EQ(hex(address.bytes), "398efb30ecc28856d97f3714af49a93b9e0a2958520316660e16ae10");
}

TEST(CardanoAddress, FromDataV3_Reward) {
    auto address = AddressV3(parse_hex("e10a84430507e150f0a06109dc3a7b1956b7a0586ae9078a55ef0e0b03"));
    EXPECT_EQ(address.string(), "stake1uy9ggsc9qls4pu9qvyyacwnmr9tt0gzcdt5s0zj4au8qkqc65geks");
    EXPECT_EQ(address.kind, AddressV3::Kind_Reward);
    EXPECT_EQ(address.networkId, AddressV3::Network_Production);
    EXPECT_EQ(hex(address.bytes), "0a84430507e150f0a06109dc3a7b1956b7a0586ae9078a55ef0e0b03");
}

TEST(CardanoAddress, FromDataV3_Invalid) {
    {   // base, invalid length
        auto address = AddressV3(parse_hex("018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34"));
        EXPECT_EQ(address.string(), "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32xsmpqws7");
        EXPECT_EQ(address.kind, AddressV3::Kind_Base);
        EXPECT_EQ(address.networkId, AddressV3::Network_Production);
        EXPECT_EQ(hex(address.bytes), "8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34");
    }
    {   // kind = 8
        auto address = AddressV3(parse_hex("818d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468"));
        EXPECT_EQ(address.string(), "addr1sxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qmxapsy");
        EXPECT_EQ(address.kind, static_cast<AddressV3::Kind>(8));
        EXPECT_EQ(address.networkId, AddressV3::Network_Production);
        EXPECT_EQ(hex(address.bytes), "8d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
}

TEST(CardanoAddress, FromPublicKeyV2) {
    {
        // caradano-crypto.js test
        auto publicKey = PublicKey(parse_hex(
                                       "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
                                       "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111" // dummy second
            ), TWPublicKeyTypeED25519Cardano);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZCxt4UV1Uj2AMMRvg5pYPypqZowVptz3GYpK4pkcvn3EjkuNH");
    }
    {
        // Adalite test account addr0
        auto publicKey = PublicKey(parse_hex(
                                       "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
                                       "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111" // dummy second
            ), TWPublicKeyTypeED25519Cardano);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");
    }
    {
        // Adalite test account addr1
        auto publicKey = PublicKey(parse_hex(
                                       "25af99056d600f7956312406bdd1cd791975bb1ae91c9d034fc65f326195fcdb247ee97ec351c0820dd12de4ca500232f73a35fe6f86778745bcd57f34d1048d"
                                       "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111" // dummy second
            ), TWPublicKeyTypeED25519Cardano);
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV");
    }
    {
        // Play1 addr0
        auto publicKey = PublicKey(parse_hex(
                                       "7cee0f30b9d536a786547dd77b35679b6830e945ffde768eb4f2a061b9dba016e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2"
                                       "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111" // dummy second
            ), TWPublicKeyTypeED25519Cardano);
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
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
            dummyKey, dummyKey, dummyKey
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        ASSERT_EQ(hex(publicKey.bytes),
                  "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03"
                  "bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
                  "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
                  "1111111111111111111111111111111111111111111111111111111111111111");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");
    }
    {
        // mnemonic Play1, addr0
        auto privateKey = PrivateKey(
            parse_hex("a089c9423100960440ccd5b7adbd202d1ab1993a7bb30fc88b287d94016df247"),
            parse_hex("da86a87f08fb15de1431a6c0ccd5ebf51c3bee81f7eaf714801bbbe4d903154a"),
            parse_hex("e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2"),
            dummyKey, dummyKey, dummyKey
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        ASSERT_EQ(hex(publicKey.bytes),
                  "7cee0f30b9d536a786547dd77b35679b6830e945ffde768eb4f2a061b9dba016"
                  "e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2"
                  "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
                  "1111111111111111111111111111111111111111111111111111111111111111");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h");
    }
    {
        // from cardano-crypto.js test
        auto privateKey = PrivateKey(
            parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
            parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
            parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"),
            dummyKey, dummyKey, dummyKey
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        ASSERT_EQ(hex(publicKey.bytes),
                  "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf698"
                  "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
                  "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
                  "1111111111111111111111111111111111111111111111111111111111111111");
        auto address = AddressV2(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZCxt4UV1Uj2AMMRvg5pYPypqZowVptz3GYpK4pkcvn3EjkuNH");
    }
    {
        // from cardano-crypto.js test
        auto privateKey = PrivateKey(
            parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
            parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
            parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"),
            dummyKey, dummyKey, dummyKey);
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        EXPECT_ANY_THROW(new AddressV2(publicKey));
    }
}

TEST(CardanoAddress, PrivateKeyExtended) {
    // check extended key lengths, private key 2x3x32 bytes, public key 2x64 bytes
    auto privateKeyExt = PrivateKey(
        parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
        parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
        parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"),
        dummyKey, dummyKey, dummyKey
    );
    auto publicKeyExt = privateKeyExt.getPublicKey(TWPublicKeyTypeED25519Cardano);
    ASSERT_EQ(128ul, publicKeyExt.bytes.size());

    // Non-extended: both are 32 bytes.
    auto privateKeyNonext = PrivateKey(
        parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"));
    auto publicKeyNonext = privateKeyNonext.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(32ul, publicKeyNonext.bytes.size());
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
    AddressV3 addr2nonleg = AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
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

TEST(CardanoAddress, StakingKey) {
    {
        auto address = AddressV3("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
        EXPECT_EQ(hex(address.data()), "01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b");
        EXPECT_EQ(address.getStakingAddress(), "stake1u80jysjtdzqt88jt4jx93h5lumfr67d273r4vwyasfa2pxcwxllmx");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).data()), "e1df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).bytes), "df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b");
    }
    {
        auto address = AddressV3("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
        EXPECT_EQ(hex(address.data()), "018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
        EXPECT_EQ(address.getStakingAddress(), "stake1u8xxf0e93w8rxr8sehvlmvp7zz6wftqg7hdplhkxyg4rg6qwgxzhc");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).data()), "e1cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).bytes), "cc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468");
    }
    {
        auto address = AddressV3("addr1q8lcljuzfg8yvpuv94x02sytmwd8jsalzf6u0j8muhq69wng9ejcvpyczmw0zx7wguq2dml4xdl2wj3k7uexsfnxep2q9ja352");
        EXPECT_EQ(hex(address.data()), "01ff8fcb824a0e46078c2d4cf5408bdb9a7943bf1275c7c8fbe5c1a2ba682e6586049816dcf11bce4700a6eff5337ea74a36f732682666c854");
        EXPECT_EQ(address.getStakingAddress(), "stake1u95zuevxqjvpdh83r08ywq9xal6nxl48fgm0wvngyenvs4qh0hqf9");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).data()), "e1682e6586049816dcf11bce4700a6eff5337ea74a36f732682666c854");
        EXPECT_EQ(hex(AddressV3(address.getStakingAddress()).bytes), "682e6586049816dcf11bce4700a6eff5337ea74a36f732682666c854");
    }
    {   // negative case: cannot get staking address from non-base address
        auto address = AddressV3("stake1u95zuevxqjvpdh83r08ywq9xal6nxl48fgm0wvngyenvs4qh0hqf9");
        EXPECT_EQ(hex(address.data()), "e1682e6586049816dcf11bce4700a6eff5337ea74a36f732682666c854");
        EXPECT_EQ(address.getStakingAddress(), "");
    }
}

} // namespace TW::Cardano::tests
