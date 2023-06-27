// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Coin.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWMnemonic.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWBase58.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWEthereum.h>
#include <TrustWalletCore/TWEthereumMessageSigner.h>
#include <TrustWalletCore/TWStarkExMessageSigner.h>
#include <TrustWalletCore/TWStarkWare.h>
#include <proto/Stellar.pb.h>

#include "HexCoding.h"

#include <gtest/gtest.h>
#include <thread>

using namespace TW;

const auto wordsStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
const auto gWords = STRING(wordsStr);
const auto gPassphrase = STRING("TREZOR");
const auto seedHex = "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29";
const auto entropyHex = "ba5821e8c356c05ba5f025d9532fe0f21f65d594";


inline void assertSeedEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto seed = WRAPD(TWHDWalletSeed(wallet.get()));
    assertHexEqual(seed, expected);
}

inline void assertMnemonicEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto mnemonic = WRAPS(TWHDWalletMnemonic(wallet.get()));
    assertStringsEqual(mnemonic, expected);
}

inline void assertEntropyEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto entropy = WRAPD(TWHDWalletEntropy(wallet.get()));
    assertHexEqual(entropy, expected);
}

TEST(HDWallet, CreateFromMnemonic) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    assertMnemonicEq(wallet, wordsStr);
    assertEntropyEq(wallet, entropyHex);
    assertSeedEq(wallet, seedHex);
}

TEST(HDWallet, CreateFromEntropy) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithEntropy(DATA(entropyHex).get(), gPassphrase.get()));
    assertMnemonicEq(wallet, wordsStr);
    assertSeedEq(wallet, seedHex);
    assertEntropyEq(wallet, entropyHex);
}

TEST(HDWallet, Generate) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, gPassphrase.get()));
    EXPECT_TRUE(TWMnemonicIsValid(WRAPS(TWHDWalletMnemonic(wallet.get())).get()));
}

TEST(HDWallet, SeedWithExtraSpaces) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    assertSeedEq(wallet, "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29");
}

TEST(HDWallet, CreateFromMnemonicNoPassword) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), STRING("").get()));
    assertSeedEq(wallet, "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
}

TEST(HDWallet, CreateFromMnemonicCheck) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonicCheck(gWords.get(), STRING("").get(), false));
    assertSeedEq(wallet, "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
}

TEST(HDWallet, CreateFromStrengthInvalid) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(64, STRING("").get()));
    ASSERT_EQ(wallet.get(), nullptr);
}

TEST(HDWallet, CreateFromMnemonicInvalid) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING("THIS IS INVALID MNEMONIC").get(), STRING("").get()));
    ASSERT_EQ(wallet.get(), nullptr);
}

TEST(HDWallet, MasterPrivateKey) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), STRING("").get()));
    auto key1 = WRAP(TWPrivateKey, TWHDWalletGetMasterKey(wallet.get(), TWCurveSECP256k1));
    auto hexKey1 = WRAPD(TWPrivateKeyData(key1.get()));

    auto key2 = WRAP(TWPrivateKey,TWHDWalletGetMasterKey(wallet.get(), TWCurveED25519));
    auto hexKey2 = WRAPD(TWPrivateKeyData(key2.get()));

    assertHexEqual(hexKey1, "d1b2b553b053f278d510a8494ead811252b1d5ec0da4434d0997a75de92bcea9");
    assertHexEqual(hexKey2, "d258c2521f7802b8e83c32f2cc97bd06b69747847390c5e247a3d19faa74202e");
}

TEST(HDWallet, Derive) {
    const auto derivationPath = TW::derivationPath(TWCoinTypeEthereum);

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key0 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));

    auto publicKey0 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key0.get(), false));
    auto publicKey0Data = WRAPD(TWPublicKeyData(publicKey0.get()));

    assertHexEqual(publicKey0Data, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
}

TEST(HDWallet, DeriveBitcoinNonextended) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), false));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "047ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b9414ec07cda7b1c9a84c2e04ea2746c21afacc5e91b47427c453c3f1a4a3e983ce5");
    // Note: address derivation does not work with nonextended public key
}

TEST(HDWallet, DeriveBitcoinExtended) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "037ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b941");

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, publicKey.get()));
    assertStringsEqual(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85");
}

TEST(HDWallet, GetKeyDerivation) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    {
        auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyDerivation(wallet.get(), TWCoinTypeBitcoin, TWDerivationBitcoinSegwit));
        assertHexEqual(WRAPD(TWPrivateKeyData(key.get())), "1901b5994f075af71397f65bd68a9fff8d3025d65f5a2c731cf90f5e259d6aac");
        auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
        auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
        assertHexEqual(publicKeyData, "037ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b941");
    }
    {
        auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyDerivation(wallet.get(), TWCoinTypeBitcoin, TWDerivationBitcoinLegacy));
        assertHexEqual(WRAPD(TWPrivateKeyData(key.get())), "28071bf4e2b0340db41b807ed8a5514139e5d6427ff9d58dbd22b7ed187103a4");
        auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
        auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
        assertHexEqual(publicKeyData, "0240ebf906b948281289405317a5eb9a98045af8a8ab5311b2e3060cfb66c507a1");
    }
}

TEST(HDWallet, DeriveAddressBitcoin) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto address = WRAP(TWString, TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeBitcoin));
    assertStringsEqual(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85");
}

TEST(HDWallet, DeriveAddressBitcoinDerivation) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    {
        auto address = WRAP(TWString, TWHDWalletGetAddressDerivation(wallet.get(), TWCoinTypeBitcoin, TWDerivationBitcoinSegwit));
        assertStringsEqual(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85");
    }
    {
        auto address = WRAP(TWString, TWHDWalletGetAddressDerivation(wallet.get(), TWCoinTypeBitcoin, TWDerivationBitcoinLegacy));
        assertStringsEqual(address, "1PeUvjuxyf31aJKX6kCXuaqxhmG78ZUdL1");
    }
}

TEST(HDWallet, DeriveEthereum) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));
    auto key2 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeSmartChain));

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), false));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key2.get(), false));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeEthereum, publicKey.get()));
    auto address2 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeSmartChain, publicKey2.get()));

    assertHexEqual(publicKeyData, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
    assertStringsEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
    assertStringsEqual(address2, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
}

TEST(HDWallet, DeriveAddressEthereum) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto address = WRAP(TWString, TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeEthereum));
    assertStringsEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
}

TEST(HDWallet, DeriveCosmos) {
    // use `gaiacli keys add key_name` to generate mnemonic words and private key
    auto words = STRING("attract term foster morning tail foam excite copper disease measure cheese camera rug enroll cause flip sword waste try local purchase between idea thank");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeCosmos));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));

    assertHexEqual(privateKeyData, "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");
}

TEST(HDWallet, DeriveNimiq) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeNimiq));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(key.get()));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "1937063865fe3294ccf3017837207bb3fea71a53720ae631b77bf9d5ca4f7f4c");
}

TEST(HDWallet, DeriveTezos) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeTezos));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(key.get()));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "c834147f97bcf95bf01f234455646a197f70b25e93089591ffde8122370ad371");
}

TEST(HDWallet, DeriveDoge) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeDogecoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "039fc134623a37c8ee44881f088a599cc44ba8a95f91f860b99d9d3b11f487e4c0");

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGPV));
    assertStringsEqual(xprv, "dgpv57ru95KiYUB5oWm2CVQH4heh1f7E9dNGdRHHVThcQkLeQ2HHxVJfFYefnpKrWZ6L6EDKJHUVq4Yyd5kPZKnRePfkCz3EzkySTydXKbgjcxN");

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGUB));
    assertStringsEqual(xpub, "dgub8rjvUmFc6cqR6NRBEj2FBZCHUDUrykPyv24Vea6bCsPex5PzNFrRtr4KN37XgwuVzzC2MikJRW2Ddcp99Ehsqp2iaU4eerNCJVruKxz6Gci");
}

TEST(HDWallet, DeriveZilliqa) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeZilliqa));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "0262746d4988c63b9972c63272461e9fa080d4dfa2a1fda3dd01285620c0a60c22");
}

TEST(HDWallet, DeriveAionPrivateKey) {
    auto words = STRING("zero behind diesel afraid edge salad drop episode high pear twin resource");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAion));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    assertHexEqual(privateKeyData, "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
}

TEST(HDWallet, DeriveFIO) {
    auto words = STRING("valley alien library bread worry brother bundle hammer loyal barely dune brave");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeFIO));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));

    uint8_t bytes[] = {0x80};
    auto prefix = WRAPD(TWDataCreateWithBytes(bytes, 1));
    TWDataAppendData(prefix.get(), privateKeyData.get());
    auto base58 = WRAPS(TWBase58Encode(prefix.get()));
    
    assertHexEqual(privateKeyData, "ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854");
    assertStringsEqual(base58, "5Kbb37EAqQgZ9vWUHoPiC2uXYhyGSFNbL6oiDp24Ea1ADxV1qnu");
}

TEST(HDWallet, DeriveAlgorand) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAlgorand));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeAlgorand, privateKey.get()));

    assertHexEqual(privateKeyData, "ce0b7ac644e2b7d9d14d3928b11643f43e48c33d3e328d059fef8add7f070e82");
}

TEST(HDWallet, DeriveMultiversX) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeMultiversX));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeMultiversX, privateKey.get()));

    assertHexEqual(privateKeyData, "0eb593141de897d60a0883320793eb49e63d556ccdf783a87ec014f150d50453");
    assertStringsEqual(address, "erd1a6l7q9cfvrgr80xuzm37tapdr4zm3mwrtl6vt8f45df45x7eadfs8ds5vv");
}

TEST(HDWallet, DeriveBinance) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBinance));
    auto key2 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeSmartChain));
    auto keyData = WRAPD(TWPrivateKeyData(key.get()));
    auto keyData2 = WRAPD(TWPrivateKeyData(key2.get()));

    auto expected = "ca81b1b0974aa063de2f74c17b9dc364a8208d105659f4f900c121fb170922fe";
    auto expectedETH = "c4f77b4a9f5a0db3a7ffc3599e61bef986037ae9a7cc1972a10d55c030270020";

    assertHexEqual(keyData, expected);
    assertHexEqual(keyData2, expectedETH);
}

TEST(HDWallet, DeriveAvalancheCChain) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAvalancheCChain));
    auto keyData = WRAPD(TWPrivateKeyData(key.get()));

    auto expectedETH = "c4f77b4a9f5a0db3a7ffc3599e61bef986037ae9a7cc1972a10d55c030270020";

    assertHexEqual(keyData, expectedETH);
}

TEST(HDWallet, DeriveCardano) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeCardano));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(TWDataSize(privateKeyData.get()), 192ul);
    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeCardano, privateKey.get()));

    assertHexEqual(privateKeyData, "f8a3b8ad30e62c369b939336c2035aba26d1ffad135e6f346f2a370517a14952e73d20aeadf906bc8b531900fb6c3ed4a05b16973c10ae24650b68b26fae4ee5d97418ba7f3b2707fae963041ff5f174195d1578da09478ad2d17a1ecc00cad478a8ca3be214870accd41f008d70e3b4b59b5981ca933d6d3f389ad317a14952166d8fd329ae3fab4712da739efc2ded9b3eef2b1a8e225dd3dddeb4f065a729b297d9fa76b8852eef235c25aac8f0ff6209ab7251f2a84c83b3b5f1161f7c59");
    assertStringsEqual(address, "addr1q9zz5nj4rqdvteauvdtc834f2vtzyrdrrdmnwdyp4s6huuz5fp33p9cq4xwpqtgguaxknzurmglv58yn9wrv6angpvvq9u36ya");
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
    
    auto emptyPrv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone));
    auto emptyPub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone));
    assertStringsEqual(emptyPrv, "");
    assertStringsEqual(emptyPub, "");
}

TEST(HDWallet, ExtendedKeysCustomAccount) {
    auto words = STRING("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto zprv0 = WRAPS(TWHDWalletGetExtendedPrivateKeyAccount(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWDerivationBitcoinSegwit, TWHDVersionZPRV, 0));
    assertStringsEqual(zprv0, "zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE");
    auto zprv1 = WRAPS(TWHDWalletGetExtendedPrivateKeyAccount(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWDerivationBitcoinSegwit, TWHDVersionZPRV, 1));
    assertStringsEqual(zprv1, "zprvAdG4iTXWBoAS2cCGuaGevCvH54GCunrvLJb2hoWCSuE3D9LS42XVg3c6sPm64w6VMq3w18vJf8nF3cBA2kUMkyWHsq6enWVXivzw42UrVHG");

    auto zpub0 = WRAPS(TWHDWalletGetExtendedPublicKeyAccount(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWDerivationBitcoinSegwit, TWHDVersionZPUB, 0));
    assertStringsEqual(zpub0, "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    auto zpub1 = WRAPS(TWHDWalletGetExtendedPublicKeyAccount(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWDerivationBitcoinSegwit, TWHDVersionZPUB, 1));
    assertStringsEqual(zpub1, "zpub6rFR7y4Q2AijF6Gk1bofHLs1d66hKFamhXWdWBup1Em25wfabZqkDqvaieV63fDQFaYmaatCG7jVNUpUiM2hAMo6SAVHcrUpSnHDpNzucB7");
}

TEST(HDWallet, PublicKeyFromX) {
    auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    auto xpubAddr2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/2").get()));
    auto xpubAddr9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/9").get()));

    auto data2 = WRAPD(TWPublicKeyData(xpubAddr2.get()));
    auto data9 = WRAPD(TWPublicKeyData(xpubAddr9.get()));

    assertHexEqual(data2, "0338994349b3a804c44bbec55c2824443ebb9e475dfdad14f4b1a01a97d42751b3");
    assertHexEqual(data9, "03786c1d274f2c804ff9a57d8e7289c281d4aef15e17187ad9f9c3722d81a6ae66");
}

TEST(HDWallet, PublicKeyInvalid) {
    auto xpub = STRING("xpub0000");
    auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/0").get()));
    ASSERT_EQ(xpubAddr.get(), nullptr);
}

TEST(HDWallet, PublicKeyFromY) {
    auto ypub = STRING("ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");
    auto ypubAddr3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(ypub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/3").get()));
    auto ypubAddr10 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(ypub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/10").get()));

    auto data3 = WRAPD(TWPublicKeyData(ypubAddr3.get()));
    auto data10 = WRAPD(TWPublicKeyData(ypubAddr10.get()));

    assertHexEqual(data3, "0299bd0bdc081a9888fac95a33e8bebcdeeb57cf7477f2f0721362f3a51a157227");
    assertHexEqual(data10, "03a39ad9c0d19bb43c45643582614298c96b0f7c9462c0de789c69013b0d609d1c");
}

TEST(HDWallet, PublicKeyFromZ) {
    auto zpub = STRING("zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    auto zpubAddr4 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/4").get()));
    auto zpubAddr11 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/11").get()));

    auto data4 = WRAPD(TWPublicKeyData(zpubAddr4.get()));
    auto data11 = WRAPD(TWPublicKeyData(zpubAddr11.get()));

    auto address4 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, zpubAddr4.get()));

    assertHexEqual(data4, "03995137c8eb3b223c904259e9b571a8939a0ec99b0717684c3936407ca8538c1b");
    assertHexEqual(data11, "0226a07edd0227fa6bc36239c0bd4db83d5e488f8fb1eeb68f89a5be916aad2d60");

    assertStringsEqual(address4, "bc1qm97vqzgj934vnaq9s53ynkyf9dgr05rargr04n");
}

TEST(HDWallet, PublicKeyFromExtended_Ethereum) {
    const auto xpub = STRING("xpub6C7LtZJgtz1BKXG9mExKUxYvX7HSF38UMMmGbpqNQw3DfYwAw8E6sH7VSVxFipvEEm2afSqTjoRgcLmycXX4zfxCWJ4HY73a9KdgvfHEQGB");
    const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeEthereum, STRING("m/44'/60'/0'/0/1").get()));
    ASSERT_NE(xpubAddr.get(), nullptr);
    auto data = WRAPD(TWPublicKeyData(xpubAddr.get()));
    ASSERT_NE(data.get(), nullptr);
    assertHexEqual(data, "044516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab513226a9d14ea37a55962ad3644a08e2ce551b4495beabb9b09e688c7b92eba18acc");
}

TEST(HDWallet, PublicKeyFromExtended_NIST256p1) {
    const auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNEO, STRING("m/44'/888'/0'/0/0").get())); // Neo
    ASSERT_NE(xpubAddr.get(), nullptr);
    auto data = WRAPD(TWPublicKeyData(xpubAddr.get()));
    ASSERT_NE(data.get(), nullptr);
    assertHexEqual(data, "03774c910fcf07fa96886ea794f0d5caed9afe30b44b83f7e213bb92930e7df4bd");
}

TEST(HDWallet, PublicKeyFromExtended_Negative) {
    const auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    {   // Ed25519
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeSolana, STRING("m/44'/501'/0'").get())); // Solana
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Ed25519Extended
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeCardano, STRING("m/1852'/1815'/0'/0/0").get())); // Cardano
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Ed25519Blake2bNano
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNano, STRING("m/44'/165'/0'").get())); // Nano
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Curve25519
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeWaves, STRING("m/44'/5741564'/0'/0'/0'").get())); // Waves
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
}

TEST(HDWallet, MultipleThreads) {
    auto passphrase = STRING("");

    auto f = [&passphrase](int n) {
        std::vector<TWString*> keys;
        for (int i = 0; i < n; i++) {
            auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, passphrase.get()));
            TWString* key = TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone);
            keys.push_back(key);
        }
        // delete returned strings
        for (int i = n-1; i >= 0; i--) {
            TWStringDelete(keys[i]);
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

TEST(HDWallet, GetDerivedKey) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), gPassphrase.get()));
    const auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetDerivedKey(wallet.get(), TWCoinTypeBitcoin, 0, 0, 0));
    const auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    assertHexEqual(privateKeyData, "1901b5994f075af71397f65bd68a9fff8d3025d65f5a2c731cf90f5e259d6aac");
}

TEST(HDWallet, GetKeyByCurve) {
    const auto derivPath = STRING("m/44'/539'/0'/0/0");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), STRING("").get()));
    const auto privateKey1 = WRAP(TWPrivateKey, TWHDWalletGetKeyByCurve(wallet.get(), TWCurveSECP256k1, derivPath.get()));
    const auto privateKeyData1 = WRAPD(TWPrivateKeyData(privateKey1.get()));
    assertHexEqual(privateKeyData1, "4fb8657d6464adcaa086d6758d7f0b6b6fc026c98dc1671fcc6460b5a74abc62");

    const auto privateKey2 = WRAP(TWPrivateKey, TWHDWalletGetKeyByCurve(wallet.get(), TWCurveNIST256p1, derivPath.get()));
    const auto privateKeyData2 = WRAPD(TWPrivateKeyData(privateKey2.get()));
    assertHexEqual(privateKeyData2, "a13df52d5a5b438bbf921bbf86276e4347fe8e2f2ed74feaaee12b77d6d26f86");
}

TEST(TWHDWallet, FromMnemonicImmutableXMainnetFromSignature) {
    // Successfully register: https://api.x.immutable.com/v1/users/0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37
    const auto mnemonic = STRING("obscure opera favorite shuffle mail tip age debate dirt pact cement loyal");
    const auto ethAddress = STRING("0xd0972E2312518Ca15A2304D56ff9cc0b7ea0Ea37");
    const auto layer = STRING("starkex");
    const auto application = STRING("immutablex");
    const auto index = STRING("1");
    const auto ethDerivationPath = STRING("m/44'/60'/0'/0/0");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), STRING("").get()));
    auto derivationPath = WRAPS(TWEthereumEip2645GetPath(ethAddress.get(), layer.get(), application.get(), index.get()));
    assertStringsEqual(derivationPath, "m/2645'/579218131'/211006541'/2124474935'/1609799702'/1");

    // Retrieve eth private key
    auto ethPrivateKey = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeEthereum, ethDerivationPath.get()));
    const auto ethPrivateKeyData = WRAPD(TWPrivateKeyData(ethPrivateKey.get()));
    assertHexEqual(ethPrivateKeyData, "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d");

    // StarkKey Derivation Path
    const auto starkDerivationPath = WRAP(TWDerivationPath, TWDerivationPathCreateWithString(derivationPath.get()));

    // Retrieve Stark Private key part
    const auto ethMsg = STRING("Only sign this request if you’ve initiated an action with Immutable X.");
    const auto ethSignature = WRAPS(TWEthereumMessageSignerSignMessageImmutableX(ethPrivateKey.get(), ethMsg.get()));
    assertStringsEqual(ethSignature, "18b1be8b78807d3326e28bc286d7ee3d068dcd90b1949ce1d25c1f99825f26e70992c5eb7f44f76b202aceded00d74f771ed751f2fe538eec01e338164914fe001");
    const auto starkPrivateKey = WRAP(TWPrivateKey, TWStarkWareGetStarkKeyFromSignature(starkDerivationPath.get(), ethSignature.get()));
    const auto starkPrivateKeyData = WRAPD(TWPrivateKeyData(starkPrivateKey.get()));
    const auto starkPubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyByType(starkPrivateKey.get(), TWPublicKeyTypeStarkex));
    const auto starkPublicKeyData = WRAPD(TWPublicKeyData(starkPubKey.get()));
    assertHexEqual(starkPrivateKeyData, "04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de");
    assertHexEqual(starkPublicKeyData, "00e5b9b11f8372610ef35d647a1dcaba1a4010716588d591189b27bf3c2d5095");

    // Account register
    const auto ethMsgToRegister = STRING("Only sign this key linking request from Immutable X");
    const auto ethSignatureToRegister = WRAPS(TWEthereumMessageSignerSignMessageImmutableX(ethPrivateKey.get(), ethMsgToRegister.get()));
    assertStringsEqual(ethSignatureToRegister, "646da4160f7fc9205e6f502fb7691a0bf63ecbb74bbb653465cd62388dd9f56325ab1e4a9aba99b1661e3e6251b42822855a71e60017b310b9f90e990a12e1dc01");
    const auto starkMsg = STRING("463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf");
    const auto starkSignature = WRAPS(TWStarkExMessageSignerSignMessage(starkPrivateKey.get(), starkMsg.get()));
    assertStringsEqual(starkSignature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528");
    ASSERT_TRUE(TWStarkExMessageSignerVerifyMessage(starkPubKey.get(), starkMsg.get(), starkSignature.get()));
}

TEST(TWHDWallet, Derive_XpubPub_vs_PrivPub) {
    // Test different routes for deriving address from mnemonic, result should be the same:
    // - Direct: mnemonic -> seed -> privateKey -> publicKey -> address
    // - Extended Public: mnemonic -> seed -> zpub -> publicKey -> address

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(gWords.get(), STRING("").get()));
    const auto coin = TWCoinTypeBitcoin;
    const auto derivPath1 = STRING("m/84'/0'/0'/0/0");
    const auto derivPath2 = STRING("m/84'/0'/0'/0/2");
    const auto expectedPublicKey1 = "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac";
    const auto expectedPublicKey2 = "031e1f64d2f6768dccb6814545b2e2d58e26ad5f91b7cbaffe881ed572c65060db";
    const auto expectedAddress1 = "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd";
    const auto expectedAddress2 = "bc1q7zddsunzaftf4zlsg9exhzlkvc5374a6v32jf6";

    // -> privateKey -> publicKey
    {
        const auto privateKey1 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), coin, derivPath1.get()));
        const auto publicKey1 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey1.get(), true));
        const auto publicKey1Data = WRAPD(TWPublicKeyData(publicKey1.get()));
        EXPECT_EQ(hex(TW::data(TWDataBytes(publicKey1Data.get()), TWDataSize(publicKey1Data.get()))), expectedPublicKey1);
        const auto address1 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey1.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWSegwitAddressDescription(address1.get())).get())), expectedAddress1);
    }
    {
        const auto privateKey2 = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), coin, derivPath2.get()));
        const auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey2.get(), true));
        const auto publicKey2Data = WRAPD(TWPublicKeyData(publicKey2.get()));
        EXPECT_EQ(hex(TW::data(TWDataBytes(publicKey2Data.get()), TWDataSize(publicKey2Data.get()))), expectedPublicKey2);
        const auto address2 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey2.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWSegwitAddressDescription(address2.get())).get())), expectedAddress2);
    }

    // zpub -> publicKey
    const auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, coin, TWHDVersionZPUB));
    EXPECT_EQ(std::string(TWStringUTF8Bytes(zpub.get())), "zpub6rNUNtxSa9Gxvm4Bdxf1MPMwrvkzwDx6vP96Hkzw3jiQKdg3fhXBStxjn12YixQB8h88B3RMSRscRstf9AEVaYr3MAqVBEWBDuEJU4PGaT9");

    {
        const auto publicKey1 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), coin, derivPath1.get()));
        EXPECT_NE(publicKey1.get(), nullptr);
        const auto publicKey1Data = WRAPD(TWPublicKeyData(publicKey1.get()));
        EXPECT_EQ(hex(TW::data(TWDataBytes(publicKey1Data.get()), TWDataSize(publicKey1Data.get()))), expectedPublicKey1);
        const auto address1 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey1.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWSegwitAddressDescription(address1.get())).get())), expectedAddress1);
    }
    {
        const auto publicKey2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), coin, derivPath2.get()));
        EXPECT_NE(publicKey2.get(), nullptr);
        const auto publicKey2Data = WRAPD(TWPublicKeyData(publicKey2.get()));
        EXPECT_EQ(hex(TW::data(TWDataBytes(publicKey2Data.get()), TWDataSize(publicKey2Data.get()))), expectedPublicKey2);
        const auto address2 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey2.get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(WRAPS(TWSegwitAddressDescription(address2.get())).get())), expectedAddress2);
    }
}
