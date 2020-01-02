// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Address.h"
#include "Cardano/Signer.h"

#include "HDWallet.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cardano;
using namespace std;


TEST(CardanoAddress, Validation) {
    // valid V2 address
    ASSERT_TRUE(Address::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx"));
    ASSERT_TRUE(Address::isValid("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W"));

    // valid V1 address
    ASSERT_TRUE(Address::isValid("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ"));
    ASSERT_TRUE(Address::isValid("DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di"));

    // invalid checksum
    ASSERT_FALSE(Address::isValid("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm"));
    // random
    ASSERT_FALSE(Address::isValid("hasoiusaodiuhsaijnnsajnsaiussai"));
    // empty
    ASSERT_FALSE(Address::isValid(""));
}

TEST(CardanoAddress, FromString) {
    {
        auto address = Address("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvx");
    }
    {
        auto address = Address("DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
        ASSERT_EQ(address.string(), "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ");
    }
}

TEST(CardanoAddress, MnemonicToAddress) {
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
            ASSERT_EQ("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W", addr);
        }
        {
            PrivateKey privKey0 = wallet.getKey(DerivationPath("m/44'/1815'/0'/0/0"));
            PublicKey pubKey0 = privKey0.getPublicKey(TWPublicKeyTypeED25519Extended);
            Address addr0 = Address(pubKey0);
            ASSERT_EQ("Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W", addr0.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(DerivationPath("m/44'/1815'/0'/0/1"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            Address addr1 = Address(pubKey1);
            ASSERT_EQ("Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV", addr1.string());
        }
        {
            PrivateKey privKey1 = wallet.getKey(DerivationPath("m/44'/1815'/0'/0/2"));
            PublicKey pubKey1 = privKey1.getPublicKey(TWPublicKeyTypeED25519Extended);
            Address addr1 = Address(pubKey1);
            ASSERT_EQ("Ae2tdPwUPEZ8LAVy21zj4BF97iWxKCmPv12W6a18zLX3V7rZDFFVgqUBkKw", addr1.string());
        }
    }
    {
        // Tested agains AdaLite
        auto mnemonicPlay1 = "youth away raise north opinion slice dash bus soldier dizzy bitter increase saddle live champion";
        auto wallet = HDWallet(mnemonicPlay1, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        ASSERT_EQ("Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h", addr);
    }
    {
        // Tested agains AdaLite
        auto mnemonicPlay2 = "return custom two home gain guilt kangaroo supply market current curtain tomorrow heavy blue robot";
        auto wallet = HDWallet(mnemonicPlay2, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        ASSERT_EQ("Ae2tdPwUPEZLtJx7LA2XZ3zzwonH9x9ieX3dMzaTBD3TfXuKczjMSjTecr1", addr);
    }
    {
        // AdaLite Demo phrase, 12-word.  AdaLite uses V1 for it, in V2 it produces different addresses.
        // In AdaLite V1 addr0 is DdzFFzCqrht7HGoJ87gznLktJGywK1LbAJT2sbd4txmgS7FcYLMQFhawb18ojS9Hx55mrbsHPr7PTraKh14TSQbGBPJHbDZ9QVh6Z6Di
        auto mnemonicALDemo = "civil void tool perfect avocado sweet immense fluid arrow aerobic boil flash";
        auto wallet = HDWallet(mnemonicALDemo, "");
        string addr = wallet.deriveAddress(TWCoinType::TWCoinTypeCardano);
        ASSERT_EQ("Ae2tdPwUPEZJbLcD8iLgN7hVGvq66WdR4zocntRekSP97Ds3MvCfmEDjJYu", addr);
    }
}

TEST(CardanoAddress, KeyHash) {
    auto xpub = parse_hex("e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000");
    auto hash = Address::keyHash(xpub);
    ASSERT_EQ("a1eda96a9952a56c983d9f49117f935af325e8a6c9d38496e945faa8", hex(hash));
}

TEST(CardanoAddress, FromPublicKey) {
    {
        // caradano-crypto.js test
        auto publicKey = PublicKey(parse_hex("e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf69869272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"), TWPublicKeyTypeED25519Extended);
        auto address = Address(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZCxt4UV1Uj2AMMRvg5pYPypqZowVptz3GYpK4pkcvn3EjkuNH");
    }
    {
        // Adalite test account addr0
        auto publicKey = PublicKey(parse_hex("57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"), TWPublicKeyTypeED25519Extended);
        auto address = Address(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ6RUCnjGHFqi59k5WZLiv3HoCCNGCW8SYc5H9srdTzn1bec4W");
    }
    {
        // Adalite test account addr1
        auto publicKey = PublicKey(parse_hex("25af99056d600f7956312406bdd1cd791975bb1ae91c9d034fc65f326195fcdb247ee97ec351c0820dd12de4ca500232f73a35fe6f86778745bcd57f34d1048d"), TWPublicKeyTypeED25519Extended);
        auto address = Address(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZ7dnds6ZyhQdmgkrDFFPSDh8jG9RAhswcXt1bRauNw5jczjpV");
    }
    {
        // Play1 addr0
        auto publicKey = PublicKey(parse_hex("7cee0f30b9d536a786547dd77b35679b6830e945ffde768eb4f2a061b9dba016e513fa1290da1d22e83a41f17eed72d4489483b561fff36b9555ffdb91c430e2"), TWPublicKeyTypeED25519Extended);
        auto address = Address(publicKey);
        ASSERT_EQ(address.string(), "Ae2tdPwUPEZJYT9g1JgQWtLveUHavyRxQGi6hVzoQjct7yyCLGgk3pCyx7h");
    }
}

TEST(CardanoAddress, FromPrivateKey) {
    {
        // mnemonic Test, addr0
        auto privateKey = PrivateKey(
            parse_hex("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744"),
            parse_hex("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff"),
            parse_hex("bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4")
        );
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Extended);
        ASSERT_EQ(hex(publicKey.bytes), "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4");
        auto address = Address(publicKey);
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
        auto address = Address(publicKey);
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
        auto address = Address(publicKey);
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
        auto address = Address("__INVALID_ADDRESS__");
    } catch (...) {
        return;
    }
    FAIL() << "Expected exception!";
}

TEST(CardanoAddress, FromStringNegativeBadChecksum) {
    try {
        auto address = Address("Ae2tdPwUPEZ18ZjTLnLVr9CEvUEUX4eW1LBHbxxxJgxdAYHrDeSCSbCxrvm");
    } catch (...) {
        return;
    }
    FAIL() << "Expected exception!";
}
