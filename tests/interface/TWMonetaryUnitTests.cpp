// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

TEST(MonetaryUnit, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = TWBitcoinAddress();
    TWBitcoinAddressInitWithPublicKey(&address, publicKey, TWP2PKHPrefixMonetaryUnit);
    auto addressString = WRAPS(TWBitcoinAddressDescription(address));
    assertStringsEqual(addressString, "7cJ2ye1qsjPCZgHuAiyDH62vHb2QsEj2BX");
}

TEST(MonetaryUnit, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true);
    auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, publicKey));
    auto string = WRAPS(TWSegwitAddressDescription(address.get()));

    assertStringsEqual(string, "mue1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as2acx2y");
}

TEST(MonetaryUnit, BuildForAddressV) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("Vw6bJFaF5Hyiveko7dpqRjVvcTAsjz7eYa").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}

TEST(MonetaryUnit, BuildForAddressE) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("ESxRxvhJP6ZKtYaMGjj48As1kgCh6hXa6X").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");
}

TEST(MonetaryUnit, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    // .bip44
    auto xpub = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeMonetaryUnit, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6DKSaEADdUgknSNMW7hECg4GnDP1VQSprsJm5tWjHNupnJeW1LjjEyFy4SrSVBtctQue7RNHCD6C3ZHG5jX3ZPJp7m8ydtfBouhp8fkDpqZ");
    assertStringsEqual(xprv, "xprv9zL6AidKo78TZxHtQ6ADqY7YEBYX5wiyVePAHW77j3NquWKMToRUhAwVDKHPwpno9WrFozNTDDkNdnX7JFXRPFJ9UFT2wgt2FXgQNkgiaqq");

    // .bip49
    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonetaryUnit, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeMonetaryUnit, TWHDVersionYPUB));

    assertStringsEqual(yprv, "yprvAKNDQHkoVVFWP4YRTpY43xAs7bN3KA7geiP9dEMVThNkk9CVrvmdNBiY3dNT4JH4LcjYDW7uDYyKva75YxJ3J52ESbdK66WfSwCNannVSmY");
    assertStringsEqual(ypub, "ypub6YMZooHhKroobYctZr54R67bfdCXicqY1wJkRcm722ujcwXeQU5suz31tvNm1BzBBdwCdCJFULHEUpneaUXEsVQSf8zWiu8ZLA8qJQiVdYp");

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonetaryUnit, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeMonetaryUnit, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAdige7bNa4K4WiJ1z7TE8b9FjhLsyAEhJS8Ea1py8WWofe4j4LtqAezMBRWnpPb2eBDCiVTUfx1WRH5PSRjfeb3pEQBDyxZiwwze2n7FBcZ");
    assertStringsEqual(zpub, "zpub6ri33d8GQRsMjCNV68zEVj5zHjBNNcxYff3qNQEagr3nYSPsbtD5iTJq2hHY4anwn7xsss9KGjaNdRqNGEUWTnQBYCSryxjVQBfuAAYsocT");
}

TEST(MonetaryUnit, DeriveFromXpub) {
    auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/14'/0'/0/2").get());
    auto pubKey9 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), STRING("m/44'/14'/0'/0/9").get());

    TWBitcoinAddress address2;
    TWBitcoinAddressInitWithPublicKey(&address2, pubKey2, TWP2PKHPrefixMonetaryUnit);
    auto address2String = WRAPS(TWBitcoinAddressDescription(address2));

    TWBitcoinAddress address9;
    TWBitcoinAddressInitWithPublicKey(&address9, pubKey9, TWP2PKHPrefixMonetaryUnit);
    auto address9String = WRAPS(TWBitcoinAddressDescription(address9));

    assertStringsEqual(address2String, "7nmtqADBwUfxhY7h7Ze35naZxwechDTMtM");
    assertStringsEqual(address9String, "7etWb3qK5zLPTQgtusYvMrLKZabdfNzMZW");
}

TEST(MonetaryUnit, DeriveFromZpub) {
    auto zpub = STRING("zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs");
    auto pubKey4 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/14'/0'/0/4").get());
    auto pubKey11 = TWHDWalletGetPublicKeyFromExtended(zpub.get(), STRING("m/44'/14'/0'/0/11").get());

    auto address4 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, pubKey4));
    auto address4String = WRAPS(TWSegwitAddressDescription(address4.get()));

    auto address11 = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPMonetaryUnit, pubKey11));
    auto address11String = WRAPS(TWSegwitAddressDescription(address11.get()));

    assertStringsEqual(address4String, "mue1qcgnevr9rp7aazy62m4gen0tfzlssa52a7qpt0z");
    assertStringsEqual(address11String, "mue1qy072y8968nzp6mz3j292h8lp72d678fcr446g8");
}

TEST(MonetaryUnit, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("7oBSZsdN6jDoEn2agojLuUqoZbjWBbcwCm").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914e3db0f7c8ce845f897bc20ce3a6cd1846066be9f88ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("ESxRxvhJP6ZKtYaMGjj48As1kgCh6hXa6X").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("Vw6bJFaF5Hyiveko7dpqRjVvcTAsjz7eYa").get()));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}
