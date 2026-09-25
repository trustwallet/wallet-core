// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "IoTeX/Address.h"
#include "Cosmos/Address.h"
#include "Coin.h"
#include "Ethereum/Address.h"
#include "Ethereum/MessageSigner.h"
#include "HDWallet.h"
#include "Hash.h"
#include "Hedera/DER.h"
#include "HexCoding.h"
#include "ImmutableX/StarkKey.h"
#include "Mnemonic.h"
#include "NEAR/Address.h"
#include "PublicKey.h"
#include "StarkEx/MessageSigner.h"
#include "TestUtilities.h"
#include "TrustWalletCore/TWEthereum.h"

#include <gtest/gtest.h>

extern std::string TESTS_ROOT;

namespace TW::HDWalletTests {

const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
const auto gPassphrase = "passphrase";

TEST(HDWallet, generate) {
    {
        HDWallet wallet = HDWallet(128, gPassphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), gPassphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 16ul);
    }
    {
        HDWallet wallet = HDWallet(256, gPassphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), gPassphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 32ul);
    }
}

TEST(HDWallet, generateInvalid) {
    EXPECT_EXCEPTION(HDWallet(64, gPassphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(129, gPassphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(512, gPassphrase), "Invalid strength");
}

TEST(HDWallet, createFromMnemonic) {
    {
        HDWallet wallet = HDWallet(mnemonic1, gPassphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), gPassphrase);
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "143cd5fc27ae46eb423efebc41610473f5e24a80f2ca2e2fa7bf167e537f58f4c68310ae487fce82e25bad29bab2530cf77fd724a5ebfc05a45872773d7ee2d6");
    }
    { // empty passphrase
        HDWallet wallet = HDWallet(mnemonic1, "");
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), "");
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
    }
}

TEST(HDWallet, entropyLength_createFromMnemonic) {
    { // 12 words
        HDWallet wallet = HDWallet("oil oil oil oil oil oil oil oil oil oil oil oil", "");
        EXPECT_EQ(wallet.getEntropy().size(), 16ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "99d33a674ce99d33a674ce99d33a674c");
    }
    { // 12 words, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
        HDWallet wallet = HDWallet("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about", "");
        EXPECT_EQ(wallet.getEntropy().size(), 16ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "00000000000000000000000000000000");
    }
    { // 15 words
        HDWallet wallet = HDWallet("history step cheap card humble screen raise seek robot slot coral roof spoil wreck caution", "");
        EXPECT_EQ(wallet.getEntropy().size(), 20ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "6c3aac9b9146ef832c4e18bb3980c0dddd25fc49");
    }
    { // 18 words
        HDWallet wallet = HDWallet("caught hockey split gun symbol code payment copy broccoli silly shed secret stove tell citizen staff photo high", "");
        EXPECT_EQ(wallet.getEntropy().size(), 24ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "246d8f48b3fdc65a2869801c791715614d6bbd8a56a0a3ad");
    }
    { // 21 words
        HDWallet wallet = HDWallet("diary shine country alpha bridge coast loan hungry hip media sell crucial swarm share gospel lake visa coin dizzy physical basket", "");
        EXPECT_EQ(wallet.getEntropy().size(), 28ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "3d58bcc40381bc59a0c37a6bf14f0d9a3db78a5933e5f4a5ad00d1f1");
    }
    { // 24 words
        HDWallet wallet = HDWallet("poet spider smile swift roof pilot subject save hand diet ice universe over brown inspire ugly wide economy symbol shove episode patient plug swamp", "");
        EXPECT_EQ(wallet.getEntropy().size(), 32ul);
        EXPECT_EQ(hex(wallet.getEntropy()), "a73a3732edebbb49f5fdfe68c7b5c0f6e9de3a1d5760faa8c771e384bf4229b6");
    }
    { // 24 words, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
        HDWallet wallet = HDWallet("letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless", "");
        EXPECT_EQ(wallet.getEntropy().size(), 32ul);
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
    EXPECT_EXCEPTION(HDWallet("THIS IS AN INVALID MNEMONIC", gPassphrase), "Invalid mnemonic");
    EXPECT_EXCEPTION(HDWallet("", gPassphrase), "Invalid mnemonic");

    EXPECT_EXCEPTION(HDWallet("", gPassphrase, false), "Invalid mnemonic");
    HDWallet walletUnchecked = HDWallet("THIS IS AN INVALID MNEMONIC", gPassphrase, false);
}

TEST(HDWallet, createFromEntropy) {
    {
        HDWallet wallet = HDWallet(parse_hex("ba5821e8c356c05ba5f025d9532fe0f21f65d594"), gPassphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
    }
}

TEST(HDWallet, createFromEntropyInvalid) {
    EXPECT_EXCEPTION(HDWallet(parse_hex(""), gPassphrase), "Invalid mnemonic data");
    EXPECT_EXCEPTION(HDWallet(parse_hex("123456"), gPassphrase), "Invalid mnemonic data");
}

TEST(HDWallet, recreateFromEntropy) {
    {
        HDWallet wallet1 = HDWallet(mnemonic1, gPassphrase);
        EXPECT_EQ(wallet1.getMnemonic(), mnemonic1);
        EXPECT_EQ(hex(wallet1.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        HDWallet wallet2 = HDWallet(wallet1.getEntropy(), gPassphrase);
        EXPECT_EQ(wallet2.getMnemonic(), wallet1.getMnemonic());
        EXPECT_EQ(wallet2.getEntropy(), wallet1.getEntropy());
        EXPECT_EQ(wallet2.getSeed(), wallet1.getSeed());
    }
}

TEST(HDWallet, privateKeyFromXPRV) {
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_TRUE(privateKey);
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::BitcoinCashAddress(publicKey);

    EXPECT_EQ(hex(publicKey.bytes), "025108168f7e5aad52f7381c18d8f880744dbee21dc02c15abe512da0b1cca7e2f");
    EXPECT_EQ(address.string(), "bitcoincash:qp3y0dyg6ya8nt4n3algazn073egswkytqs00z7rz4");
}

TEST(HDWallet, privateKeyFromXPRV_Invalid) {
    const std::string xprv = "xprv9y0000";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_InvalidVersion) {
    {
        // Version bytes (first 4) are invalid, 0x00000000
        const std::string xprv = "11117pE7xwz2GARukXY8Vj2ge4ozfX4HLgy5ztnJXjr5btzJE8EbtPhZwrcPWAodW2aFeYiXkXjSxJYm5QrnhSKFXDgACcFdMqGns9VLqESCq3";
        auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
    {
        // Version bytes (first 4) are invalid, 0xdeadbeef
        const std::string xprv = "pGoh3VZXR4mTkT4bfqj4paog12KmHkAWkdLY8HNsZagD1ihVccygLr1ioLBhVQsny47uEh5swP3KScFc4JJrazx1Y7xvzmH2y5AseLgVMwomBTg2";
        auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
}

TEST(HDWallet, privateKeyFromExtended_InvalidCurve) {
    // invalid coin & curve, should fail
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinType(123456), DerivationPath(TWPurposeBIP44, 123456, 0, 0, 0));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_Invalid45) {
    // 45th byte is not 0
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbhw2dJ8QexahgVSfkjxU4FgmN4GLGN3Ui8oLqC6433CeyPUNVHHh";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromMptv) {
    const std::string mptv = "Mtpv7SkyM349Svcf1WiRtB5hC91ZZkVsGuv3kz1V7tThGxBFBzBLFnw6LpaSvwpHHuy8dAfMBqpBvaSAHzbffvhj2TwfojQxM7Ppm3CzW67AFL5";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(mptv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 4));
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
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(zprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoin), 0, 0, 5));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::SegwitAddress(publicKey, "bc");

    EXPECT_EQ(hex(publicKey.bytes), "022dc3f5a3fcfd2d1cc76d0cb386eaad0e30247ba729da0d8847a2713e444fdafa");
    EXPECT_EQ(address.string(), "bc1q5yyq60jepll68hds7exa7kpj20gsvdu0aztw5x");
}

TEST(HDWallet, privateKeyFromDGRV) {
    const std::string dgpv = "dgpv595jAJYGBLanByCJXRzrWBZFVXdNisfuPmKRDquCQcwBbwKbeR21AtkETf4EpjBsfsK3kDZgMqhcuky1B9PrT5nxiEcjghxpUVYviHXuCmc";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(dgpv, TWCoinTypeDogecoin, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDogecoin), 0, 0, 1));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::Address(publicKey, TW::p2pkhPrefix(TWCoinTypeDogecoin));

    EXPECT_EQ(hex(publicKey.bytes), "03eb6bf281990ee074a39c71ed8ce78c486066ac433bcf066dd5eb08f87d3a6c34");
    EXPECT_EQ(address.string(), "D5taDndQJ1fDF3AM1yWavmJY2BgSi17CUv");
}

TEST(HDWallet, privateKeyFromXPRVForDGB) {
    const std::string xprvForDgb = "xprv9ynLofyuR3uCqCMJADwzBaPnXB53EVe5oLujvPfdvCxae3NzgEpYjZMgcUeS8EUeYfYVLG61ZgPXm9TZWiwBnLVCgd551vCwpXC19hX3mFJ";
    auto privateKey = HDWallet<>::getPrivateKeyFromExtended(xprvForDgb, TWCoinTypeDigiByte, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDigiByte), 0, 0, 1));
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
    const std::string vectorsJsonPath = std::string(TESTS_ROOT) + "/common/HDWallet/bip39_vectors.json";
    auto vectorsJson = loadJson(vectorsJsonPath)["english"];
    return vectorsJson;
}

TEST(HDWallet, Bip39Vectors) {
    // BIP39 test vectors, from https://github.com/trezor/python-mnemonic/blob/master/vectors.json
    const auto passphrase = "TREZOR";
    const auto vectors = getVectors();
    for (const auto& v : vectors) {
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

TEST(HDWallet, getKeyByCurve) {
    const auto derivPath = "m/44'/539'/0'/0/0";
    HDWallet wallet = HDWallet(mnemonic1, "");
    {
        const auto privateKey = wallet.getKeyByCurve(TWCurveSECP256k1, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "4fb8657d6464adcaa086d6758d7f0b6b6fc026c98dc1671fcc6460b5a74abc62");
    }
    {
        const auto privateKey = wallet.getKeyByCurve(TWCurveNIST256p1, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "a13df52d5a5b438bbf921bbf86276e4347fe8e2f2ed74feaaee12b77d6d26f86");
    }
}

TEST(HDWallet, getKey) {
    const auto derivPath = "m/44'/539'/0'/0/0";
    HDWallet wallet = HDWallet(mnemonic1, "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeBitcoin, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "4fb8657d6464adcaa086d6758d7f0b6b6fc026c98dc1671fcc6460b5a74abc62");
    }
    {
        const auto privateKey = wallet.getKey(TWCoinTypeNEO, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "a13df52d5a5b438bbf921bbf86276e4347fe8e2f2ed74feaaee12b77d6d26f86");
    }
}

TEST(HDWallet, AptosKey) {
    const auto derivPath = "m/44'/637'/0'/0'/0'";
    HDWallet wallet = HDWallet(mnemonic1, "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeAptos, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "7f2634c0e2414a621e96e39c41d09021700cee12ee43328ed094c5580cd0bd6f");
        EXPECT_EQ(hex(privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes), "633e5c7e355bdd484706436ce1f06fdf280bd7c2229a7f9b6489684412c6967c");
    }
}

TEST(HDWallet, HederaKey) {
    // https://github.com/hashgraph/hedera-sdk-js/blob/e0cd39c84ab189d59a6bcedcf16e4102d7bb8beb/packages/cryptography/test/unit/Mnemonic.js#L47
    {
        const auto derivPath = "m/44'/3030'/0'/0'/0";
        HDWallet wallet = HDWallet("inmate flip alley wear offer often piece magnet surge toddler submit right radio absent pear floor belt raven price stove replace reduce plate home", "");
        {
            const auto privateKey = wallet.getKey(TWCoinTypeHedera, DerivationPath(derivPath));
            EXPECT_EQ(Hedera::gHederaDerPrefixPrivate + hex(privateKey.bytes), "302e020100300506032b657004220420853f15aecd22706b105da1d709b4ac05b4906170c2b9c7495dff9af49e1391da");
            EXPECT_EQ(Hedera::gHederaDerPrefixPublic + hex(privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes), "302a300506032b6570032100b63b3815f453cf697b53b290b1d78e88c725d39bde52c34c79fb5b4c93894673");
        }
    }
    {
        const auto derivPath = "m/44'/3030'/0'/0'/0";
        HDWallet wallet = HDWallet("walk gun glide frequent exhaust sugar siege prosper staff skill swarm label", "");
        {
            const auto privateKey = wallet.getKey(TWCoinTypeHedera, DerivationPath(derivPath));
            EXPECT_EQ(Hedera::gHederaDerPrefixPrivate + hex(privateKey.bytes), "302e020100300506032b657004220420650c5120cbdc6244e3d10001eb27eea4dd3f80c331b3b6969fa434797d4edd50");
            EXPECT_EQ(Hedera::gHederaDerPrefixPublic + hex(privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes), "302a300506032b65700321007df3e1ab790b28de4706d36a7aa99a0e043cb3e2c3d6ec6686e4af7f638b0860");
        }
    }
}

TEST(HDWallet, FromSeedStark) {
    auto seed = parse_hex("4c4a250231bcac7beb165aec4c9b049b4ba40ad8dd287dc79b92b1ffcf20cdcf");
    ASSERT_EQ(load(seed), uint256_t("34506778598894488719068064129252410649539581100963007245393949841529394744783"));
    auto derivationPath = DerivationPath("m/2645'/579218131'/211006541'/1534045311'/1431804530'/1");
    auto key = HDWallet<32>::bip32DeriveRawSeed(TWCoinTypeEthereum, seed, derivationPath);
    ASSERT_EQ(hex(key.bytes), "57384e99059bb1c0e51d70f0fca22d18d7191398dd39d6b9b4e0521174b2377a");
    auto addr = Ethereum::Address(key.getPublicKey(TWPublicKeyTypeSECP256k1Extended)).string();
    ASSERT_EQ(addr, "0x47bbe762944B089315ac50c9ca762F4B4884B965");
}

TEST(HDWallet, FromMnemonicStark) {
    // https://github.com/starkware-libs/starkware-crypto-utils/blob/d3a1e655105afd66ebc07f88a179a3042407cc7b/test/js/key_derivation.spec.js#L20
    const auto mnemonic = "range mountain blast problem vibrant void vivid doctor cluster enough melody salt layer language laptop boat major space monkey unit glimpse pause change vibrant";
    const std::string ethAddress = "0xA4864D977b944315389d1765Ffa7E66F74eE8cD7";
    HDWallet wallet = HDWallet(mnemonic, "");
    const auto& res = WRAPS(TWEthereumEip2645GetPath(STRING(ethAddress.c_str()).get(), STRING("starkex").get(), STRING("starkdeployement").get(), STRING("0").get()));
    auto derivationPath = DerivationPath(TWStringUTF8Bytes(res.get()));
    ASSERT_EQ(derivationPath.string(), "m/2645'/579218131'/891216374'/1961790679'/2135936222'/0");

    // ETH
    {
        auto ethPrivKey = wallet.getKey(TWCoinTypeEthereum, DerivationPath("m/44'/60'/0'/0/0"));
        auto ethAddressFromPub = Ethereum::Address(ethPrivKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)).string();
        ASSERT_EQ(ethAddressFromPub, ethAddress);
    }

    // Stark
    {
        auto starkPrivKey = wallet.getKeyByCurve(TWCurveStarkex, DerivationPath(derivationPath));
        auto starkPubKey  = hex(starkPrivKey.getPublicKey(TWPublicKeyTypeStarkex).bytes);
        ASSERT_EQ(hex(starkPrivKey.bytes), "06cf0a8bf113352eb863157a45c5e5567abb34f8d32cddafd2c22aa803f4892c");
        ASSERT_EQ(starkPubKey, "02d2bbdc1adaf887b0027cdde2113cfd81c60493aa6dc15d7887ddf1a82bc831");
    }
}

TEST(HDWallet, FromMnemonicImmutableX) {
    // Successfully register the user: https://api.sandbox.x.immutable.com/v1/users/0x1A817D0cC495C8157E4C734c48a1e840473CBCa1
    const auto mnemonic = "owner erupt swamp room swift final allow unaware hint identify figure cotton";
    const auto ethAddress = "0x1A817D0cC495C8157E4C734c48a1e840473CBCa1";
    HDWallet wallet = HDWallet(mnemonic, "");
    const auto& res = WRAPS(TWEthereumEip2645GetPath(STRING(ethAddress).get(), STRING("starkex").get(), STRING("immutablex").get(), STRING("1").get()));
    auto derivationPath = DerivationPath(TWStringUTF8Bytes(res.get()));
    ASSERT_EQ(derivationPath.string(), "m/2645'/579218131'/211006541'/1195162785'/289656960'/1");

    // ETH
    {
        auto ethPrivKey = wallet.getKey(TWCoinTypeEthereum, DerivationPath("m/44'/60'/0'/0/0"));
        ASSERT_EQ(hex(ethPrivKey.bytes), "a84f129929f6effe3fd541bcaa8a13d80714cd93c205682bea8b9e0cfc28a2ad");
        auto ethAddressFromPub = Ethereum::Address(ethPrivKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)).string();
        ASSERT_EQ(ethAddressFromPub, ethAddress);
    }

    // Stark
    {
        auto starkPrivKey = wallet.getKeyByCurve(TWCurveStarkex, DerivationPath(derivationPath));
        auto starkPubKey  = starkPrivKey.getPublicKey(TWPublicKeyTypeStarkex);
        ASSERT_EQ(hex(starkPrivKey.bytes), "02d037bb9c1302295c2f9fa66bcc4ab8e353a3140600a390598777d69c1bc71a");
        ASSERT_EQ(hex(starkPubKey.bytes), "006c061ea4195769058e0e2e14cd747619a866954a412e15fa2241fdf49438cf");

        auto starkMsg = "28419a504c5b1c83df4fdcbf7f5f36a7d5cfa8148aff2d33aed2f40a64e7ea0";
        auto starkSignature = StarkEx::MessageSigner::signMessage(starkPrivKey, starkMsg);
        ASSERT_EQ(starkSignature, "077cae8f00327a2072d3ca8b31725263f61303dc0142a631561d33cb2b4cb221008d659541d59f1589b0e714ddc0a5bee77faddf093f96d529b6c55c0bffd45d");
        ASSERT_TRUE(StarkEx::MessageSigner::verifyMessage(starkPubKey, starkMsg, starkSignature));
    }
}

TEST(HDWallet, FromMnemonicImmutableXMainnet) {
    const auto mnemonic = "ocean seven canyon push fiscal banana music guess arrange edit glance school";
    const auto ethAddress = "0x39E652fE9458D391737058b0dd5eCC6ec910A7dd";
    HDWallet wallet = HDWallet(mnemonic, "");
    const auto& res = WRAPS(TWEthereumEip2645GetPath(STRING(ethAddress).get(), STRING("starkex").get(), STRING("immutablex").get(), STRING("1").get()));
    auto derivationPath = DerivationPath(TWStringUTF8Bytes(res.get()));
    ASSERT_EQ(derivationPath.string(), "m/2645'/579218131'/211006541'/1225828317'/985503965'/1");

    // ETH
    {
        auto ethPrivKey = wallet.getKey(TWCoinTypeEthereum, DerivationPath("m/44'/60'/0'/0/0"));
        ASSERT_EQ(hex(ethPrivKey.bytes), "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84");
        auto ethAddressFromPub = Ethereum::Address(ethPrivKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)).string();
        ASSERT_EQ(ethAddressFromPub, ethAddress);

        std::string tosign = "Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3";
        auto hexEthSignature = Ethereum::MessageSigner::signMessage(ethPrivKey, tosign, Ethereum::MessageType::ImmutableX);

        ASSERT_EQ(hexEthSignature, "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01");
    }

    // Stark
    {
        auto starkPrivKey = wallet.getKeyByCurve(TWCurveStarkex, DerivationPath(derivationPath));
        auto starkPubKey  = starkPrivKey.getPublicKey(TWPublicKeyTypeStarkex);
        ASSERT_EQ(hex(starkPrivKey.bytes), "070128376c2cfd21e7475708049d00c83d7ab65f15368e28730bf1684dee8370");
        ASSERT_EQ(hex(starkPubKey.bytes), "00453ca02b347f80e5ddfc4caf254852fc05b172b37bca8f7e28600631d12dfe");

        auto starkMsg = "76b66c453cd1b812032ff206a28df59f6abe41e805b9f1c48a1c4afe780756c";
        auto starkSignature = StarkEx::MessageSigner::signMessage(starkPrivKey, starkMsg);
        ASSERT_EQ(starkSignature, "070ad88f79650fbdc152affd738d4ec29888bed554ea74f9ad8ca7031ef300b50597f4a62752336db06e6d37dfc18047fdd40804f5fd19cebfda8cac91e4f178");
        ASSERT_TRUE(StarkEx::MessageSigner::verifyMessage(starkPubKey, starkMsg, starkSignature));
    }
}

TEST(HDWallet, FromMnemonicImmutableXMainnetFromSignature) {
    // Successfully register: https://api.x.immutable.com/v1/users/0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37
    const auto mnemonic = "obscure opera favorite shuffle mail tip age debate dirt pact cement loyal";
    const auto ethAddress = "0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37";
    HDWallet wallet = HDWallet(mnemonic, "");
    const auto& res = WRAPS(TWEthereumEip2645GetPath(STRING(ethAddress).get(), STRING("starkex").get(), STRING("immutablex").get(), STRING("1").get()));
    auto derivationPath = DerivationPath(TWStringUTF8Bytes(res.get()));
    ASSERT_EQ(derivationPath.string(), "m/2645'/579218131'/211006541'/2124474935'/1609799702'/1");

    // ETH + stark
    {
        auto ethPrivKey = wallet.getKey(TWCoinTypeEthereum, DerivationPath("m/44'/60'/0'/0/0"));
        ASSERT_EQ(hex(ethPrivKey.bytes), "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d");
        auto ethAddressFromPub = Ethereum::Address(ethPrivKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended)).string();
        ASSERT_EQ(ethAddressFromPub, ethAddress);
        auto signature = Ethereum::MessageSigner::signMessage(ethPrivKey, "Only sign this request if you’ve initiated an action with Immutable X.", Ethereum::MessageType::ImmutableX);
        ASSERT_EQ(signature, "18b1be8b78807d3326e28bc286d7ee3d068dcd90b1949ce1d25c1f99825f26e70992c5eb7f44f76b202aceded00d74f771ed751f2fe538eec01e338164914fe001");
        auto starkPrivKey = ImmutableX::getPrivateKeyFromRawSignature(parse_hex(signature), DerivationPath(derivationPath));
        auto starkPubKey  = starkPrivKey.getPublicKey(TWPublicKeyTypeStarkex);
        ASSERT_EQ(hex(starkPrivKey.bytes), "04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de");
        ASSERT_EQ(hex(starkPubKey.bytes), "00e5b9b11f8372610ef35d647a1dcaba1a4010716588d591189b27bf3c2d5095");
        auto signatureToSend = Ethereum::MessageSigner::signMessage(ethPrivKey, "Only sign this key linking request from Immutable X", Ethereum::MessageType::ImmutableX);
        ASSERT_EQ(signatureToSend, "646da4160f7fc9205e6f502fb7691a0bf63ecbb74bbb653465cd62388dd9f56325ab1e4a9aba99b1661e3e6251b42822855a71e60017b310b9f90e990a12e1dc01");

        auto starkMsg = "463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf";
        auto starkSignature = StarkEx::MessageSigner::signMessage(starkPrivKey, starkMsg);
        ASSERT_EQ(starkSignature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528");
        ASSERT_TRUE(StarkEx::MessageSigner::verifyMessage(starkPubKey, starkMsg, starkSignature));
    }
}

TEST(HDWallet, StargazeKey) {
    const auto derivPath = "m/44'/118'/0'/0/0";
    HDWallet wallet = HDWallet("rude segment two fury you output manual volcano sugar draft elite fame", "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeStargaze, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
        const auto p = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(p.bytes), "02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5");
        EXPECT_EQ(Cosmos::Address(TWCoinTypeStargaze ,p).string(), "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy");
    }
}

TEST(HDWallet, CoreumKey) {
    const auto derivPath = "m/44'/990'/0'/0/0";
    HDWallet wallet = HDWallet("rude segment two fury you output manual volcano sugar draft elite fame", "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeCoreum, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "56e5e45bf33a779527ec670b5336f6bc78efbe0e3bf1f004e7250673a82a3431");
        const auto p = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(p.bytes), "0345d8d927b955c3cd468d12b5bc634c7919ee4777e578439af6314cf04b2ff114");
        EXPECT_EQ(Cosmos::Address(TWCoinTypeCoreum ,p).string(), "core1a5nvz6smgsph9gephguyhn30fmzrpaxrvvdjun");
    }
}

TEST(HDWallet, NearKey) {
    const auto derivPath = "m/44'/397'/0'";
    HDWallet wallet = HDWallet("owner erupt swamp room swift final allow unaware hint identify figure cotton", "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeNEAR, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "35e0d9631bd538d5569266abf6be7a9a403ebfda92ddd49b3268e35360a6c2dd");
        const auto p = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        EXPECT_EQ(hex(p.bytes), "b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf");
        EXPECT_EQ(NEAR::Address(p).string(), "b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf");
    }
}

TEST(HDWallet, IoTexEvmKeys) {
    const auto derivPath = "m/44'/304'/0'/0/0";
    HDWallet wallet = HDWallet("token major laundry actor dish lunch physical machine kingdom adapt gym true", "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeEthereum, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "3aa86eafa99cb9ae0f7c1c4f06391ffbef91578169715dfbdcdf76b532b73f24");
        const auto p = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        EXPECT_EQ(hex(p.bytes), "042be00e86db75bbe3e8defe9bb09fbd5444eea10e2d53d55468f3d25bf3b0cb3ea8d992baba30c9353584b8ff061f8585cae1c792b8bb6f0607750dbf4fe8c760");
        EXPECT_EQ(Ethereum::Address(p).string(), "0x6b3FBEDcB9E106e84c3a47f63cf96Df8500bBc22");
    }
}

TEST(HDWallet, IoTexKeys) {
    const auto derivPath = "m/44'/304'/0'/0/0";
    HDWallet wallet = HDWallet("token major laundry actor dish lunch physical machine kingdom adapt gym true", "");
    {
        const auto privateKey = wallet.getKey(TWCoinTypeIoTeX, DerivationPath(derivPath));
        EXPECT_EQ(hex(privateKey.bytes), "3aa86eafa99cb9ae0f7c1c4f06391ffbef91578169715dfbdcdf76b532b73f24");
        const auto p = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
        EXPECT_EQ(hex(p.bytes), "042be00e86db75bbe3e8defe9bb09fbd5444eea10e2d53d55468f3d25bf3b0cb3ea8d992baba30c9353584b8ff061f8585cae1c792b8bb6f0607750dbf4fe8c760");
        EXPECT_EQ(IoTeX::Address(p).string(), "io1dvlmah9euyrwsnp6glmre7tdlpgqh0pzz542zd");
    }
    // io1qmkv62pvg56qkashkwauhhjv3gtjhcm889r8dc
}

} // namespace TW::HDWalletTests
