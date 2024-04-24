// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.dydx

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestDydxAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val key = PrivateKey("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433".toHexByteArray())
        val pubKey = key.getPublicKeySecp256k1(true)
        val address = AnyAddress(pubKey, CoinType.DYDX)
        val expected = AnyAddress("dydx1mry47pkga5tdswtluy0m8teslpalkdq0hc72uz", CoinType.DYDX)

        assertEquals(address.description(), expected.description())
    }
}
