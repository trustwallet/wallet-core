// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWData.h>
#include "../interface/TWTestUtilities.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

TEST(TWCardano, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA(
        "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4"
        "639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a"
        ).get()));
    ASSERT_NE(nullptr, privateKey.get());
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519Extended(privateKey.get()));
    ASSERT_NE(nullptr, publicKey.get());
    ASSERT_EQ(128, publicKey.get()->impl.bytes.size());
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeCardano));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t");

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t").get(), TWCoinTypeCardano));
    ASSERT_NE(nullptr, address2.get());
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));
    assertStringsEqual(address2String, "addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t");

    ASSERT_TRUE(TWAnyAddressEqual(address.get(), address2.get()));
}
