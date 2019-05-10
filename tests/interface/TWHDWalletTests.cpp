// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Coin.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <proto/Stellar.pb.h>
#include <Stellar/Signer.h>

#include "HexCoding.h"

#include <gtest/gtest.h>
#include <thread>

auto words = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
auto passphrase = STRING("TREZOR");

auto valid = STRING("credit expect life fade cover suit response wash pear what skull force");
auto invalidWord = STRING("ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow");
auto invalidWord1 = STRING("high culture ostrich wrist exist ignore interest hybridous exclude width more");
auto invalidChecksum = STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow");
auto invalidWordCount = STRING("credit expect life fade cover suit response wash what skull force");

inline void assertSeedEq(std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    auto seed = WRAPD(TWHDWalletSeed(wallet.get()));
    assertHexEqual(seed, expected);
}

TEST(HDWallet, Seed) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    assertSeedEq(wallet, "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29");
}

TEST(HDWallet, IsValid) {
    EXPECT_TRUE(TWHDWalletIsValid(valid.get()));
    EXPECT_TRUE(TWHDWalletIsValid(STRING("rebuild park fatigue flame one clap grocery scheme upon symbol rifle flush brave feed clutch").get()));
}

TEST(HDWallet, InvalidWord) {
    EXPECT_FALSE(TWHDWalletIsValid(invalidWord.get()));
}

TEST(HDWallet, InvalidWord1) {
    EXPECT_FALSE(TWHDWalletIsValid(invalidWord1.get()));
}

TEST(HDWallet, InvalidChecksum) {
    EXPECT_FALSE(TWHDWalletIsValid(invalidChecksum.get()));
}

TEST(HDWallet, InvalidWordCount) {
    EXPECT_FALSE(TWHDWalletIsValid(invalidWordCount.get()));
}

TEST(HDWallet, SeedWithExtraSpaces) {
    auto words = STRING("ripple scissors  kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal\n");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    assertSeedEq(wallet, "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29");
}

TEST(HDWallet, SeedNoPassword) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    assertSeedEq(wallet, "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
}

TEST(HDWallet, Derive) {
    const auto derivationPath = TW::derivationPath(TWCoinTypeEthereum);

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key0 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));

    auto publicKey0 = TWPrivateKeyGetPublicKeySecp256k1(key0.get(), false);
    auto publicKey0Data = WRAPD(TWPublicKeyData(publicKey0));

    assertHexEqual(publicKey0Data, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
}

TEST(HDWallet, DeriveBitcoin) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), false);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "047ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b9414ec07cda7b1c9a84c2e04ea2746c21afacc5e91b47427c453c3f1a4a3e983ce5");
}

TEST(HDWallet, DeriveEthereum) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), false);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeEthereum, publicKey));

    assertHexEqual(publicKeyData, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
    assertStringsEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
}

TEST(HDWallet, DeriveCosmos) {
    // use `gaiacli keys add key_name` to generate mnemonic words and private key
    auto words = STRING("attract term foster morning tail foam excite copper disease measure cheese camera rug enroll cause flip sword waste try local purchase between idea thank");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeCosmos));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));

    assertHexEqual(privateKeyData, "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");

    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");
}

TEST(HDWallet, DeriveNimiq) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeNimiq));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(key.get());
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "1937063865fe3294ccf3017837207bb3fea71a53720ae631b77bf9d5ca4f7f4c");
}

TEST(HDWallet, DeriveTezos) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeTezos));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(key.get());
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "c834147f97bcf95bf01f234455646a197f70b25e93089591ffde8122370ad371");
}

TEST(HDWallet, DeriveDoge) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeDogecoin));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), true);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "039fc134623a37c8ee44881f088a599cc44ba8a95f91f860b99d9d3b11f487e4c0");

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGPV));
    assertStringsEqual(xprv, "dgpv57ru95KiYUB5oWm2CVQH4heh1f7E9dNGdRHHVThcQkLeQ2HHxVJfFYefnpKrWZ6L6EDKJHUVq4Yyd5kPZKnRePfkCz3EzkySTydXKbgjcxN");

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGUB));
    assertStringsEqual(xpub, "dgub8rjvUmFc6cqR6NRBEj2FBZCHUDUrykPyv24Vea6bCsPex5PzNFrRtr4KN37XgwuVzzC2MikJRW2Ddcp99Ehsqp2iaU4eerNCJVruKxz6Gci");
}

TEST(HDWallet, DeriveZilliqa) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeZilliqa));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(key.get(), true);
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey));

    assertHexEqual(publicKeyData, "0262746d4988c63b9972c63272461e9fa080d4dfa2a1fda3dd01285620c0a60c22");
}

TEST(HDWallet, DeriveAionPrivateKey) {
    auto words = STRING("zero behind diesel afraid edge salad drop episode high pear twin resource");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAion));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    assertHexEqual(privateKeyData, "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
}

TEST(HDWallet, ExtendedKeys) {
    auto words = STRING("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoin, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoin, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9xpXFhFpqdQK3TmytPBqXtGSwS3DLjojFhTGht8gwAAii8py5X6pxeBnQ6ehJiyJ6nDjWGJfZ95WxByFXVkDxHXrqu53WCRGypk2ttuqncb");
    assertStringsEqual(xpub, "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");

    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeBitcoin, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeBitcoin, TWHDVersionYPUB));
    assertStringsEqual(yprv, "yprvAHwhK6RbpuS3dgCYHM5jc2ZvEKd7Bi61u9FVhYMpgMSuZS613T1xxQeKTffhrHY79hZ5PsskBjcc6C2V7DrnsMsNaGDaWev3GLRQRgV7hxF");
    assertStringsEqual(ypub, "ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");

    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE");
    assertStringsEqual(zpub, "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
}

TEST(HDWallet, PublicKeyFromX) {
    auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    auto xpubAddr2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/145'/0'/0/2").get());
    auto xpubAddr9 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/145'/0'/0/9").get());

    auto data2 = WRAPD(TWPublicKeyData(xpubAddr2));
    auto data9 = WRAPD(TWPublicKeyData(xpubAddr9));

    assertHexEqual(data2, "0338994349b3a804c44bbec55c2824443ebb9e475dfdad14f4b1a01a97d42751b3");
    assertHexEqual(data9, "03786c1d274f2c804ff9a57d8e7289c281d4aef15e17187ad9f9c3722d81a6ae66");
}

TEST(HDWallet, PublicKeyFromY) {
    auto ypub = STRING("ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");
    auto ypubAddr3 = TWHDWalletGetPublicKeyFromExtended(ypub.get(), STRING("m/44'/0'/0'/0/3").get());
    auto ypubAddr10 = TWHDWalletGetPublicKeyFromExtended(ypub.get(), STRING("m/44'/0'/0'/0/10").get());

    auto data3 = WRAPD(TWPublicKeyData(ypubAddr3));
    auto data10 = WRAPD(TWPublicKeyData(ypubAddr10));

    assertHexEqual(data3, "0299bd0bdc081a9888fac95a33e8bebcdeeb57cf7477f2f0721362f3a51a157227");
    assertHexEqual(data10, "03a39ad9c0d19bb43c45643582614298c96b0f7c9462c0de789c69013b0d609d1c");
}

TEST(HDWallet, PublicKeyFromZ) {
    auto zpub = STRING("zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    auto zpubAddr4 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/0'/0'/0/4").get());
    auto zpubAddr11 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/0'/0'/0/11").get());

    auto data4 = WRAPD(TWPublicKeyData(zpubAddr4));
    auto data11 = WRAPD(TWPublicKeyData(zpubAddr11));

    auto address4 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, zpubAddr4));

    assertHexEqual(data4, "03995137c8eb3b223c904259e9b571a8939a0ec99b0717684c3936407ca8538c1b");
    assertHexEqual(data11, "0226a07edd0227fa6bc36239c0bd4db83d5e488f8fb1eeb68f89a5be916aad2d60");

    assertStringsEqual(address4, "bc1qm97vqzgj934vnaq9s53ynkyf9dgr05rargr04n");
}

TEST(HDWallet, MultipleThreads) {
    auto passphrase = STRING("");

    auto f = [&passphrase](int n) {
        for (int i = 0; i < n; i++) {
            auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, passphrase.get()));
            TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone);
        }
    };

    // Ensure multiple threads cause no asserts
    std::thread th1(f, 10);
    std::thread th2(f, 10);
    std::thread th3(f, 10);

    th1.join();
    th2.join();
    th3.join();
}
