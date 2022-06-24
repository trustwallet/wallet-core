// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"
#include "Mnemonic.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Ethereum/Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "Hash.h"
#include "Base58.h"
#include "Coin.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

extern std::string TESTS_ROOT;

namespace TW {

const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
const auto passphrase = "passphrase";

TEST(HDWallet, generate) {
    {
        HDWallet wallet = HDWallet(128, passphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 16);
    }
    {
        HDWallet wallet = HDWallet(256, passphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 32);
    }
}

TEST(HDWallet, generateInvalid) {
    EXPECT_EXCEPTION(HDWallet(64, passphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(129, passphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(512, passphrase), "Invalid strength");
}

TEST(HDWallet, createFromMnemonic) {
    {
        HDWallet wallet = HDWallet(mnemonic1, passphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "143cd5fc27ae46eb423efebc41610473f5e24a80f2ca2e2fa7bf167e537f58f4c68310ae487fce82e25bad29bab2530cf77fd724a5ebfc05a45872773d7ee2d6");
    }
    {   // empty passphrase
        HDWallet wallet = HDWallet(mnemonic1, "");
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), "");
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
    }
}

TEST(HDWallet, entropyLength_createFromMnemonic) {
    {   // 12 words
        HDWallet wallet = HDWallet("oil oil oil oil oil oil oil oil oil oil oil oil", "");
        EXPECT_EQ(wallet.getEntropy().size(), 16);
        EXPECT_EQ(hex(wallet.getEntropy()), "99d33a674ce99d33a674ce99d33a674c");
    }
    {   // 12 words, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
        HDWallet wallet = HDWallet("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about", "");
        EXPECT_EQ(wallet.getEntropy().size(), 16);
        EXPECT_EQ(hex(wallet.getEntropy()), "00000000000000000000000000000000");
    }
    {   // 15 words
        HDWallet wallet = HDWallet("history step cheap card humble screen raise seek robot slot coral roof spoil wreck caution", "");
        EXPECT_EQ(wallet.getEntropy().size(), 20);
        EXPECT_EQ(hex(wallet.getEntropy()), "6c3aac9b9146ef832c4e18bb3980c0dddd25fc49");
    }
    {   // 18 words
        HDWallet wallet = HDWallet("caught hockey split gun symbol code payment copy broccoli silly shed secret stove tell citizen staff photo high", "");
        EXPECT_EQ(wallet.getEntropy().size(), 24);
        EXPECT_EQ(hex(wallet.getEntropy()), "246d8f48b3fdc65a2869801c791715614d6bbd8a56a0a3ad");
    }
    {   // 21 words
        HDWallet wallet = HDWallet("diary shine country alpha bridge coast loan hungry hip media sell crucial swarm share gospel lake visa coin dizzy physical basket", "");
        EXPECT_EQ(wallet.getEntropy().size(), 28);
        EXPECT_EQ(hex(wallet.getEntropy()), "3d58bcc40381bc59a0c37a6bf14f0d9a3db78a5933e5f4a5ad00d1f1");
    }
    {   // 24 words
        HDWallet wallet = HDWallet("poet spider smile swift roof pilot subject save hand diet ice universe over brown inspire ugly wide economy symbol shove episode patient plug swamp", "");
        EXPECT_EQ(wallet.getEntropy().size(), 32);
        EXPECT_EQ(hex(wallet.getEntropy()), "a73a3732edebbb49f5fdfe68c7b5c0f6e9de3a1d5760faa8c771e384bf4229b6");
    }
    {   // 24 words, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
        HDWallet wallet = HDWallet("letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless", "");
        EXPECT_EQ(wallet.getEntropy().size(), 32);
        EXPECT_EQ(hex(wallet.getEntropy()), "8080808080808080808080808080808080808080808080808080808080808080");
    }
}

TEST(HDWallet, createFromSpanishMnemonic) {
    {
        EXPECT_EXCEPTION(HDWallet("llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut", ""), "Invalid mnemonic");
    }
    {
        HDWallet wallet = HDWallet("llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut", "", false);
        EXPECT_EQ(wallet.getMnemonic(), "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut");
        EXPECT_EQ(wallet.getPassphrase(), "");
        EXPECT_EQ(hex(wallet.getEntropy()), "");
        EXPECT_EQ(hex(wallet.getSeed()), "ec8f8703432fc7d32e699ee056e9d84b1435e6a64a6a40ad63dbde11eab189a276ddcec20f3326d3c6ee39cbd018585b104fc3633b801c011063ae4c318fb9b6");
    }
}

TEST(HDWallet, createFromMnemonicInvalid) {
    EXPECT_EXCEPTION(HDWallet("THIS IS AN INVALID MNEMONIC", passphrase), "Invalid mnemonic");
    EXPECT_EXCEPTION(HDWallet("", passphrase), "Invalid mnemonic");

    EXPECT_EXCEPTION(HDWallet("", passphrase, false), "Invalid mnemonic");
    HDWallet walletUnchecked = HDWallet("THIS IS AN INVALID MNEMONIC", passphrase, false);
}

TEST(HDWallet, createFromEntropy) {
    {
        HDWallet wallet = HDWallet(parse_hex("ba5821e8c356c05ba5f025d9532fe0f21f65d594"), passphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
    }
}

TEST(HDWallet, createFromEntropyInvalid) {
    EXPECT_EXCEPTION(HDWallet(parse_hex(""), passphrase), "Invalid mnemonic data");
    EXPECT_EXCEPTION(HDWallet(parse_hex("123456"), passphrase), "Invalid mnemonic data");
}

TEST(HDWallet, recreateFromEntropy) {
    {
        HDWallet wallet1 = HDWallet(mnemonic1, passphrase);
        EXPECT_EQ(wallet1.getMnemonic(), mnemonic1);
        EXPECT_EQ(hex(wallet1.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        HDWallet wallet2 = HDWallet(wallet1.getEntropy(), passphrase);
        EXPECT_EQ(wallet2.getMnemonic(), wallet1.getMnemonic());
        EXPECT_EQ(wallet2.getEntropy(), wallet1.getEntropy());
        EXPECT_EQ(wallet2.getSeed(), wallet1.getSeed());
    }
}

TEST(HDWallet, privateKeyFromXPRV) {
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_TRUE(privateKey);
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::BitcoinCashAddress(publicKey);

    EXPECT_EQ(hex(publicKey.bytes), "025108168f7e5aad52f7381c18d8f880744dbee21dc02c15abe512da0b1cca7e2f");
    EXPECT_EQ(address.string(), "bitcoincash:qp3y0dyg6ya8nt4n3algazn073egswkytqs00z7rz4");
}

TEST(HDWallet, privateKeyFromXPRV_Invalid) {
    const std::string xprv = "xprv9y0000";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_InvalidVersion) {
    {
        // Version bytes (first 4) are invalid, 0x00000000
        const std::string xprv = "11117pE7xwz2GARukXY8Vj2ge4ozfX4HLgy5ztnJXjr5btzJE8EbtPhZwrcPWAodW2aFeYiXkXjSxJYm5QrnhSKFXDgACcFdMqGns9VLqESCq3";
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
    {
        // Version bytes (first 4) are invalid, 0xdeadbeef
        const std::string xprv = "pGoh3VZXR4mTkT4bfqj4paog12KmHkAWkdLY8HNsZagD1ihVccygLr1ioLBhVQsny47uEh5swP3KScFc4JJrazx1Y7xvzmH2y5AseLgVMwomBTg2";
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
}

TEST(HDWallet, privateKeyFromExtended_InvalidCurve) {
    // invalid coin & curve, should fail
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinType(123456), DerivationPath(TWPurposeBIP44, 123456, 0, 0, 0));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_Invalid45) {
    // 45th byte is not 0
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbhw2dJ8QexahgVSfkjxU4FgmN4GLGN3Ui8oLqC6433CeyPUNVHHh";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromMptv) {
    const std::string mptv = "Mtpv7SkyM349Svcf1WiRtB5hC91ZZkVsGuv3kz1V7tThGxBFBzBLFnw6LpaSvwpHHuy8dAfMBqpBvaSAHzbffvhj2TwfojQxM7Ppm3CzW67AFL5";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(mptv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 4));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);

    auto witness = Data{0x00, 0x14};
    auto keyHash = Hash::sha256ripemd(publicKey.bytes.data(), 33);
    witness.insert(witness.end(), keyHash.begin(), keyHash.end());

    auto prefix = Data{TW::p2shPrefix(TWCoinTypeLitecoin)};
    auto redeemScript = Hash::sha256ripemd(witness.data(), witness.size());
    prefix.insert(prefix.end(), redeemScript.begin(), redeemScript.end());

    auto address = Bitcoin::Address(prefix);

    EXPECT_EQ(hex(publicKey.bytes), "02c36f9c3051e9cfbb196ecc35311f3ad705ea6798ffbe6b039e70f6bd047e6f2c");
    EXPECT_EQ(address.string(), "MBzcCaoLk9626cLj2UVvcxs6nsVUi39zEy");
}

TEST(HDWallet, privateKeyFromZprv) {
    const std::string zprv = "zprvAdzGEQ44z4WPLNCRpDaup2RumWxLGgR8PQ9UVsSmJigXsHVDaHK1b6qGM2u9PmxB2Gx264ctAz4yRoN3Xwf1HZmKcn6vmjqwsawF4WqQjfd";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(zprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoin), 0, 0, 5));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::SegwitAddress(publicKey, "bc");

    EXPECT_EQ(hex(publicKey.bytes), "022dc3f5a3fcfd2d1cc76d0cb386eaad0e30247ba729da0d8847a2713e444fdafa");
    EXPECT_EQ(address.string(), "bc1q5yyq60jepll68hds7exa7kpj20gsvdu0aztw5x");
}

TEST(HDWallet, privateKeyFromDGRV) {
    const std::string dgpv = "dgpv595jAJYGBLanByCJXRzrWBZFVXdNisfuPmKRDquCQcwBbwKbeR21AtkETf4EpjBsfsK3kDZgMqhcuky1B9PrT5nxiEcjghxpUVYviHXuCmc";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(dgpv, TWCoinTypeDogecoin, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDogecoin), 0, 0, 1));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::Address(publicKey, TW::p2pkhPrefix(TWCoinTypeDogecoin));

    EXPECT_EQ(hex(publicKey.bytes), "03eb6bf281990ee074a39c71ed8ce78c486066ac433bcf066dd5eb08f87d3a6c34");
    EXPECT_EQ(address.string(), "D5taDndQJ1fDF3AM1yWavmJY2BgSi17CUv");
}

TEST(HDWallet, privateKeyFromXPRVForDGB) {
    const std::string xprvForDgb = "xprv9ynLofyuR3uCqCMJADwzBaPnXB53EVe5oLujvPfdvCxae3NzgEpYjZMgcUeS8EUeYfYVLG61ZgPXm9TZWiwBnLVCgd551vCwpXC19hX3mFJ";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprvForDgb, TWCoinTypeDigiByte, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDigiByte), 0, 0, 1));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::Address(publicKey, TW::p2pkhPrefix(TWCoinTypeDigiByte));

    EXPECT_EQ(hex(publicKey.bytes), "03238a5c541c2cbbf769dbe0fb2a373c22db4da029370767fbe746d59da4de07f1");
    EXPECT_EQ(address.string(), "D9Gv7jWSVsS9Y5q98C79WyfEj6P2iM5Nzs");
}

TEST(HDWallet, DeriveWithLeadingZerosEth) {
    // Derivation test case with leading zeroes, see  https://blog.polychainlabs.com/bitcoin,/bip32,/bip39,/kdf/2021/05/17/inconsistent-bip32-derivations.html
    const auto mnemonic = "name dash bleak force moral disease shine response menu rescue more will";
    const auto derivationPath = "m/44'/60'";
    const auto coin = TWCoinTypeEthereum;
    auto wallet = HDWallet(mnemonic, "");
    const auto addr = Ethereum::Address(wallet.getKey(coin, DerivationPath(derivationPath)).getPublicKey(TW::publicKeyType(coin)));
    EXPECT_EQ(addr.string(), "0x0ba17e928471c64AaEaf3ABfB3900EF4c27b380D");
}

static nlohmann::json getVectors() {
    const std::string vectorsJsonPath = std::string(TESTS_ROOT) + "/HDWallet/bip39_vectors.json";
    auto vectorsJson = loadJson(vectorsJsonPath)["english"];
    return vectorsJson;
}

TEST(HDWallet, Bip39Vectors) {
    // BIP39 test vectors, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
    const auto passphrase = "TREZOR";
    const auto vectors = getVectors();
    for (const auto& v: vectors) {
        const std::string entropy = v[0];
        const std::string mnemonic = v[1];
        const std::string seed = v[2];
        const std::string xprv = v[3];
        { // from mnemonic
            HDWallet wallet = HDWallet(mnemonic, passphrase);
            EXPECT_EQ(wallet.getMnemonic(), mnemonic);
            EXPECT_EQ(wallet.getPassphrase(), passphrase);
            EXPECT_EQ(hex(wallet.getEntropy()), entropy);
            EXPECT_EQ(hex(wallet.getSeed()), seed);
            EXPECT_EQ(wallet.getRootKey(TWCoinTypeBitcoin, TWHDVersionXPRV), xprv);
        }
        { // from entropy
            HDWallet wallet = HDWallet(parse_hex(entropy), passphrase);
            EXPECT_EQ(wallet.getMnemonic(), mnemonic);
            EXPECT_EQ(wallet.getPassphrase(), passphrase);
            EXPECT_EQ(hex(wallet.getEntropy()), entropy);
            EXPECT_EQ(hex(wallet.getSeed()), seed);
            EXPECT_EQ(wallet.getRootKey(TWCoinTypeBitcoin, TWHDVersionXPRV), xprv);
        }
    }
}

TEST(HDWallet, getExtendedPrivateKey) {
    const HDWallet wallet = HDWallet(mnemonic1, "");
    const auto purpose = TWPurposeBIP44;
    const auto coin = TWCoinTypeBitcoin;
    const auto hdVersion = TWHDVersionZPRV;
    
    // default
    const auto extPubKey1 = wallet.getExtendedPrivateKey(purpose, coin, hdVersion);
    EXPECT_EQ(extPubKey1, "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    // explicitly specify default account=0
    const auto extPubKey2 = wallet.getExtendedPrivateKeyAccount(purpose, coin, TWDerivationDefault, hdVersion, 0);
    EXPECT_EQ(extPubKey2, "zprvAcwsTZNaY1f7rfwsy5GseSDStYBrxwtsBZDkb3iyuQUs4NF6n58BuH7Xj54RuaSCWtU5CiQzuYQgFgqr1HokgKcVAeGeXokhJUAJeP3VmvY");

    // custom account=1
    const auto extPubKey3 = wallet.getExtendedPrivateKeyAccount(purpose, coin, TWDerivationDefault, hdVersion, 1);
    EXPECT_EQ(extPubKey3, "zprvAcwsTZNaY1f7sifgNNgdNa4P9mPtyg3zRVgwkx2qF9Sn7F255MzP6Zyumn6bgV5xuoS8ZrDvjzE7APcFSacXdzFYpGvyybb1bnAoh5nHxpn");
}

TEST(HDWallet, getExtendedPublicKey) {
    const HDWallet wallet = HDWallet(mnemonic1, "");
    const auto purpose = TWPurposeBIP44;
    const auto coin = TWCoinTypeBitcoin;
    const auto hdVersion = TWHDVersionZPUB;
    const auto derivation = TWDerivationDefault;
    
    // default
    const auto extPubKey1 = wallet.getExtendedPublicKey(purpose, coin, hdVersion);
    EXPECT_EQ(extPubKey1, "zpub6qwDs4uUNPDR5A2M56ot1aABSa2MNQciYn9MPS8bTk1qwAaFKcSST5S1aLidvPp9twqpaumG7vikR2vHhBXjp5oGgHyMvWK3AtUkfeEgyns");

    // explicitly specify default account=0
    const auto extPubKey2 = wallet.getExtendedPublicKeyAccount(purpose, coin, derivation, hdVersion, 0);
    EXPECT_EQ(extPubKey2, "zpub6qwDs4uUNPDR5A2M56ot1aABSa2MNQciYn9MPS8bTk1qwAaFKcSST5S1aLidvPp9twqpaumG7vikR2vHhBXjp5oGgHyMvWK3AtUkfeEgyns");

    // custom account=1
    const auto extPubKey3 = wallet.getExtendedPublicKeyAccount(purpose, coin, derivation, hdVersion, 1);
    EXPECT_EQ(extPubKey3, "zpub6qwDs4uUNPDR6Ck9UQDdji17hoEPP8mqnicYZLSSoUykz3MDcuJdeNJPd3BozqEafeLZkegWqzAvkgA4JZZ5tTN2rDpGKfk54essyfx1eZP");
}

TEST(HDWallet, Derive_XpubPub_vs_PrivPub) {
    // Test different routes for deriving address from mnemonic, result should be the same:
    // - Direct: mnemonic -> seed -> privateKey -> publicKey -> address
    // - Extended Public: mnemonic -> seed -> zpub -> publicKey -> address
    // - Extended Private: mnemonic -> seed -> zpriv -> privateKey -> publicKey -> address

    const HDWallet wallet = HDWallet(mnemonic1, "");
    const auto coin = TWCoinTypeBitcoin;
    const auto derivPath1 = DerivationPath("m/84'/0'/0'/0/0");
    const auto derivPath2 = DerivationPath("m/84'/0'/0'/0/2");
    const auto expectedPublicKey1 = "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac";
    const auto expectedPublicKey2 = "031e1f64d2f6768dccb6814545b2e2d58e26ad5f91b7cbaffe881ed572c65060db";
    const auto expectedAddress1 = "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd";
    const auto expectedAddress2 = "bc1q7zddsunzaftf4zlsg9exhzlkvc5374a6v32jf6";

    // -> privateKey -> publicKey
    {
        const auto privateKey1 = wallet.getKey(coin, derivPath1);
        const auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(publicKey1.bytes), expectedPublicKey1);
        const auto address1 = Bitcoin::SegwitAddress(publicKey1, "bc");
        EXPECT_EQ(address1.string(), expectedAddress1);
    }
    {
        const auto privateKey2 = wallet.getKey(coin, derivPath2);
        const auto publicKey2 = privateKey2.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(publicKey2.bytes), expectedPublicKey2);
        const auto address2 = Bitcoin::SegwitAddress(publicKey2, "bc");
        EXPECT_EQ(address2.string(), expectedAddress2);
    }

    // zpub -> publicKey
    const auto zpub = wallet.getExtendedPublicKey(TWPurposeBIP84, coin, TWHDVersionZPUB);
    EXPECT_EQ(zpub, "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9");

    {
        const auto publicKey1 = wallet.getPublicKeyFromExtended(zpub, coin, derivPath1);
        EXPECT_TRUE(publicKey1.has_value());
        EXPECT_EQ(hex(publicKey1->bytes), expectedPublicKey1);
        const auto address1 = Bitcoin::SegwitAddress(publicKey1.value(), "bc");
        EXPECT_EQ(address1.string(), expectedAddress1);
    }
    {
        const auto publicKey2 = wallet.getPublicKeyFromExtended(zpub, coin, derivPath2);
        EXPECT_TRUE(publicKey2.has_value());
        EXPECT_EQ(hex(publicKey2->bytes), expectedPublicKey2);
        const auto address2 = Bitcoin::SegwitAddress(publicKey2.value(), "bc");
        EXPECT_EQ(address2.string(), expectedAddress2);
    }

    // zpriv -> privateKey -> publicKey
    const auto zpriv = wallet.getExtendedPrivateKey(TWPurposeBIP84, coin, TWHDVersionZPRV);
    EXPECT_EQ(zpriv, "zprvAdP7yPRYjmifiGyiXw7zzFRDJtvWXmEFZADVVNbKVQBRSqLu8ACvu6eFvhrnnw4QwdTD8PUVa48MguwiPTiyfn85zWx9iA5MYy4Eufu5bas");

    {
        const auto privateKey1 = wallet.getPrivateKeyFromExtended(zpriv, coin, derivPath1);
        EXPECT_TRUE(privateKey1.has_value());
        const auto publicKey1 = privateKey1->getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(publicKey1.bytes), expectedPublicKey1);
        const auto address1 = Bitcoin::SegwitAddress(publicKey1, "bc");
        EXPECT_EQ(address1.string(), expectedAddress1);
    }
    {
        const auto privateKey2 = wallet.getPrivateKeyFromExtended(zpriv, coin, derivPath2);
        EXPECT_TRUE(privateKey2.has_value());
        const auto publicKey2 = privateKey2->getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(publicKey2.bytes), expectedPublicKey2);
        const auto address2 = Bitcoin::SegwitAddress(publicKey2, "bc");
        EXPECT_EQ(address2.string(), expectedAddress2);
    }
}

} // namespace
