// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
