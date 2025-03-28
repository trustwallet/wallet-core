// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.scroll

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestScrollAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("828c4c48c2cef521f0251920891ed79e871faa24f64f43cde83d07bc99f8dbf0".toHexByteArray(), CoinType.SCROLL.curve())
        val pubkey = key.getPublicKeySecp256k1(false)
        val address = AnyAddress(pubkey, CoinType.SCROLL)
        val expected = AnyAddress("0xe32DC46bfBF78D1eada7b0a68C96903e01418D64", CoinType.SCROLL)

        assertEquals(address.description(), expected.description())
    }
}
