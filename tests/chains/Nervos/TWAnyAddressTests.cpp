// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "PrivateKey.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnyAddressNervos, AddressFromPublicKey) {
    auto privateKey =
        WRAP(TWPrivateKey,
             TWPrivateKeyCreateWithData(
                 DATA("8a2a726c44e46d1efaa0f9c2a8efed932f0e96d6050b914fde762ee285e61feb").get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    ASSERT_NE(nullptr, publicKey.get());
    ASSERT_EQ(33ul, publicKey.get()->impl.bytes.size());
    auto address =
        WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNervos));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwy"
                                      "k5x9erg8furras980hksatlslfaktks7epf25");
}

TEST(TWAnyAddressNervos, AddressFromString) {
    auto address =
        WRAP(TWAnyAddress,
             TWAnyAddressCreateWithString(STRING("ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthy"
                                                 "waa50xwsqwyk5x9erg8furras980hksatlslfaktks7epf25")
                                              .get(),
                                          TWCoinTypeNervos));
    ASSERT_NE(nullptr, address.get());
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwy"
                                      "k5x9erg8furras980hksatlslfaktks7epf25");
}

TEST(TWAnyAddressNervos, AddressFromWallet) {
    auto wallet = WRAP(
        TWHDWallet,
        TWHDWalletCreateWithMnemonic(
            STRING(
                "alpha draw toss question picnic endless recycle wrong enable roast success palm")
                .get(),
            STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeNervos));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(TWDataSize(privateKeyData.get()), 32ul);

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
    EXPECT_EQ(TWDataSize(publicKeyData.get()), 33ul);
    assertHexEqual(publicKeyData,
                   "026c9e4cbb95d4b3a123c1fc80795feacc38029683a1b3e16bccf49bba25fb2858");

    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeNervos, privateKey.get()));
    assertStringsEqual(address, "ckb1qzda0cr08m85hc8jlnfp3zer7xulejywt49kt2rr0vthywaa50xwsqwyk5x9er"
                                "g8furras980hksatlslfaktks7epf25");
}
