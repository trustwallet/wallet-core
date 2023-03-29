// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>

namespace TW::Litecoin::tests {

TEST(Litecoin, LegacyAddress) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWBitcoinAddress, TWBitcoinAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeP2pkhPrefix(TWCoinTypeLitecoin)));
    auto addressString = WRAPS(TWBitcoinAddressDescription(address.get()));
    assertStringsEqual(addressString, "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS");
}

TEST(Litecoin, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeLitecoin));
    auto string = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(string, "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu");
}

TEST(Litecoin, LockScriptForAddressL) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("LgKiekick9Ka7gYoYzAWGrEq8rFBJzYiyf").get(), TWCoinTypeLitecoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}

TEST(Litecoin, LockScriptForAddressM) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("MHhghmmCTASDnuwpgsPUNJVPTFaj61GzaG").get(), TWCoinTypeLitecoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");
}

TEST(Litecoin, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
                                       STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
                                       STRING("TREZOR").get()));

    // .bip44
    auto lptv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeLitecoin, TWHDVersionLTPV));
    auto ltub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeLitecoin, TWHDVersionLTUB));

    assertStringsEqual(lptv, "Ltpv77Tkf73NsKY3NQWVr6sHXeDQHWV6EVBXStcyxfnwEVebFuz1URxi16SBzj4v7n3mSYh8PQXhSFM2aXNFdx8bvWBLZGXeik3UQXeXn5vudGj");
    assertStringsEqual(ltub, "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu");

    // .bip49
    auto mptv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeLitecoin, TWHDVersionMTPV));
    auto mtub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeLitecoin, TWHDVersionMTUB));

    assertStringsEqual(mptv, "Mtpv7SPQ3PnRFU5yMidTBbXKxb6pgrE1Ny1yVssVvTz8VLDppPrhdydSaoMp6fm58VbtBTrVZVacMrSUim44RccBLu8NFAqj7ZaB5JBzb8cgQHp");
    assertStringsEqual(mtub, "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw");

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeLitecoin, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeLitecoin, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAeCuQd5okFvvK1oeAQEPtgtPd5feXtcmszyCDK8HUPob28R79tUgtpCga79KgtDkUBn72AMig5NNzUCFY1JeRsZcEitDVEYuB48uHt2YEDB");
    assertStringsEqual(zpub, "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs");
}

TEST(Litecoin, DeriveFromZpub) {
    auto zpub = STRING("zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs");
    auto pubKey4 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeLitecoin, STRING("m/44'/2'/0'/0/4").get()));
    auto pubKey11 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeLitecoin, STRING("m/44'/2'/0'/0/11").get()));

    auto address4 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey4.get(), TWCoinTypeLitecoin));
    auto address4String = WRAPS(TWAnyAddressDescription(address4.get()));

    auto address11 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey11.get(), TWCoinTypeLitecoin));
    auto address11String = WRAPS(TWAnyAddressDescription(address11.get()));

    assertStringsEqual(address4String, "ltc1qcgnevr9rp7aazy62m4gen0tfzlssa52axwytt6");
    assertStringsEqual(address11String, "ltc1qy072y8968nzp6mz3j292h8lp72d678fcmms6vl");
}

TEST(Litecoin, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("ltc1qs32zgdhe2tpzcnz55r7d9jvhce33063sfht3q0").get(), TWCoinTypeLitecoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "001484542436f952c22c4c54a0fcd2c997c66317ea30");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("MHhghmmCTASDnuwpgsPUNJVPTFaj61GzaG").get(), TWCoinTypeLitecoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9146b85b3dac9340f36b9d32bbacf2ffcb0851ef17987");

    auto script3 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("LgKiekick9Ka7gYoYzAWGrEq8rFBJzYiyf").get(), TWCoinTypeLitecoin));
    auto scriptData3 = WRAPD(TWBitcoinScriptData(script3.get()));
    assertHexEqual(scriptData3, "76a914e771c6695c5dd189ccc4ef00cd0f3db3096d79bd88ac");
}

} // namespace TW::Litecoin::tests
