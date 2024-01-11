// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(Nano, DeriveAddress) {
    // Amongst other things, test that the mnemonic is compatible between different wallets
    auto words = STRING("edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur");
    auto passphrase = STRING("some password");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeNano));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Blake2b(key.get()));

    ASSERT_EQ(TW::hex(key->impl.bytes), "3be4fc2ef3f3b7374e6fc4fb6e7bb153f8a2998b3b3dab50853eabe128024143");
    ASSERT_EQ(TW::hex(publicKey.get()->impl.bytes), "5b65b0e8173ee0802c2c3e6c9080d1a16b06de1176c938a924f58670904e82c4");

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNano));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressString, "nano_1pu7p5n3ghq1i1p4rhmek41f5add1uh34xpb94nkbxe8g4a6x1p69emk8y1d");
}
