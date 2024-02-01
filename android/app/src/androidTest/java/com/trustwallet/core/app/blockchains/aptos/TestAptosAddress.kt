// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.aptos

import org.junit.Assert.assertFalse
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestAptosAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val any = AnyAddress("0x6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108", CoinType.APTOS)
        assertEquals(any.coin(), CoinType.APTOS)
        assertEquals(any.description(), "0x6af7d07b8a541913dfa87a9f99628faa255c70241ef9ebd9b82a7e715ee13108")

        assertFalse(AnyAddress.isValid("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4", CoinType.APTOS))
    }
}
