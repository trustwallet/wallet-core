// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWCardano.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWCardano.h>
#include "TestUtilities.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

TEST(TWCardano, AddressFromPublicKey) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
        "639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a"
        ).get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Cardano(privateKey.get()));
    ASSERT_NE(nullptr, publicKey.get());
    ASSERT_EQ(128ul, publicKey.get()->impl.bytes.size());
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeCardano));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t");

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t").get(), TWCoinTypeCardano));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));
    assertStringsEqual(address2String, "addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t");

    ASSERT_TRUE(TWAnyAddressEqual(address.get(), address2.get()));
}

TEST(TWCardano, AddressFromWallet) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("cost dash dress stove morning robust group affair stomach vacant route volume yellow salute laugh").get(),
        STRING("").get()
    ));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeCardano));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    EXPECT_EQ(TWDataSize(privateKeyData.get()), 192ul);
    
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Cardano(privateKey.get()));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));
    EXPECT_EQ(TWDataSize(publicKeyData.get()), 128ul);
    assertHexEqual(publicKeyData, "fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26faf4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276");

    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeCardano, privateKey.get()));
    assertStringsEqual(address, "addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
}

TEST(TWCardano, GetStakingKey) {
    {
        auto stakingAddress = WRAPS(TWCardanoGetStakingAddress(STRING("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23").get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(stakingAddress.get())), "stake1u80jysjtdzqt88jt4jx93h5lumfr67d273r4vwyasfa2pxcwxllmx");
    }
    {   // negative case: cannot get staking address from non-base address
        auto stakingAddress = WRAPS(TWCardanoGetStakingAddress(STRING("stake1u95zuevxqjvpdh83r08ywq9xal6nxl48fgm0wvngyenvs4qh0hqf9").get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(stakingAddress.get())), "");
    }
    {   // negative case: cannot get staking address from invalid address, should not throw
        auto stakingAddress = WRAPS(TWCardanoGetStakingAddress(STRING("__THIS_IS_NOT_A_VALID_CARDANO_ADDRESS__").get()));
        EXPECT_EQ(std::string(TWStringUTF8Bytes(stakingAddress.get())), "");
    }
}
