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
        val key = PrivateKey("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111".toHexByteArray())
        val pubkey = key.publicKeyEd25519Extended
        val address = AnyAddress(pubkey, CoinType.CARDANO)
        val expected = AnyAddress("addr1qx4z6twzknkkux0hhp0kq6hvdfutczp56g56y5em8r8mgvqgu83fq6ft8wnx2h6e039x78ul8a04yqlt4jg4378pszts5a80gr", CoinType.CARDANO)

        assertEquals(pubkey.data().toHex(), "0x57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f31111111111111111111111111111111111111111111111111111111111111111")
        assertEquals(address.description(), expected.description())
    }
}
