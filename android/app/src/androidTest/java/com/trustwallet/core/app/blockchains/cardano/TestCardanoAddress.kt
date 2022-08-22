// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.cardano

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestCardanoAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a".toHexByteArray())
        val pubkey = key.publicKeyEd25519Cardano
        val address = AnyAddress(pubkey, CoinType.CARDANO)
        val expected = AnyAddress("addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvxalp7nkkk25vuspleke2zltaetmlwrfxv7t049cq9jmwjswmfw6t", CoinType.CARDANO)

        assertEquals(pubkey.data().toHex(), "0x57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d483e4b09a4ba73725625c346870e109bd335831f2ba0b72b09bfb44040f1016caed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a")
        assertEquals(address.description(), expected.description())
    }
}
