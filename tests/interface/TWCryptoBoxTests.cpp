// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "TrustWalletCore/TWCryptoBox.h"
#include "TrustWalletCore/TWCryptoBoxPublicKey.h"
#include "TrustWalletCore/TWCryptoBoxSecretKey.h"

#include <gtest/gtest.h>

TEST(TWCryptoBox, EncryptDecryptEasy) {
    const auto mySecret = WRAP(TWCryptoBoxSecretKey, TWCryptoBoxSecretKeyCreate());
    const auto myPubkey = WRAP(TWCryptoBoxPublicKey, TWCryptoBoxSecretKeyGetPublicKey(mySecret.get()));

    const auto otherSecret = WRAP(TWCryptoBoxSecretKey, TWCryptoBoxSecretKeyCreate());
    const auto otherPubkey = WRAP(TWCryptoBoxPublicKey, TWCryptoBoxSecretKeyGetPublicKey(otherSecret.get()));

    const auto message = "Well done is better than well said. -Benjamin Franklin";
    const auto messageData = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(message), strlen(message)));

    const auto encrypted = WRAPD(TWCryptoBoxEncryptEasy(mySecret.get(), otherPubkey.get(), messageData.get()));

    // Step 2. Make sure the Box can be decrypted by the other side.
    const auto decrypted = WRAPD(TWCryptoBoxDecryptEasy(otherSecret.get(), myPubkey.get(), encrypted.get()));
    const auto decryptedData = dataFromTWData(decrypted.get());
    std::string decryptedMessage(decryptedData->begin(), decryptedData->end());

    EXPECT_EQ(decryptedMessage, message);
}

TEST(TWCryptoBox, PublicKeyWithData) {
    auto pubkeyBytesHex = "afccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747";
    auto pubkeyBytes = DATA(pubkeyBytesHex);

    ASSERT_TRUE(TWCryptoBoxPublicKeyIsValid(pubkeyBytes.get()));
    const auto publicKey = WRAP(TWCryptoBoxPublicKey, TWCryptoBoxPublicKeyCreateWithData(pubkeyBytes.get()));
    const auto actualBytes = WRAPD(TWCryptoBoxPublicKeyData(publicKey.get()));
    assertHexEqual(actualBytes, pubkeyBytesHex);
}

TEST(TWCryptoBox, SecretKeyWithData) {
    auto secretBytesHex = "dd87000d4805d6fbd89ae1352f5e4445648b79d5e901c92aebcb610e9be468e4";
    auto secretBytes = DATA(secretBytesHex);

    ASSERT_TRUE(TWCryptoBoxSecretKeyIsValid(secretBytes.get()));
    const auto publicKey = WRAP(TWCryptoBoxSecretKey, TWCryptoBoxSecretKeyCreateWithData(secretBytes.get()));
    const auto actualBytes = WRAPD(TWCryptoBoxSecretKeyData(publicKey.get()));
    assertHexEqual(actualBytes, secretBytesHex);
}

TEST(TWCryptoBox, DecryptEasyError) {
    auto otherPubkeyBytes = DATA("afccabc5b28a8a1fd1cd880516f9c854ae2498d0d1b978b53a59f38e4ae55747");

    const auto mySecret = WRAP(TWCryptoBoxSecretKey, TWCryptoBoxSecretKeyCreate());
    const auto otherPubkey = WRAP(TWCryptoBoxPublicKey, TWCryptoBoxPublicKeyCreateWithData(otherPubkeyBytes.get()));

    // The given encrypted box cannot be decrypted by using `mySecret` and `otherPubkey`.
    const auto invalidEncrypted = DATA("7a7b9c8fee6e3c597512848c7d513e7131193cdfd410ff6611522fdeea99d7160873182019d7a18502f22c5e3644d26a2b669365");

    const auto* decrypted = TWCryptoBoxDecryptEasy(mySecret.get(), otherPubkey.get(), invalidEncrypted.get());
    ASSERT_EQ(decrypted, nullptr);
}
