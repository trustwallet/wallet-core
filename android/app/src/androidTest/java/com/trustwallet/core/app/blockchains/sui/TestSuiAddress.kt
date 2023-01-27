// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.sui

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestSuiAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val any = AnyAddress("0x061ce2b2100a71bb7aa0da98998887ad82597948", CoinType.SUI)
        assertEquals(any.coin(), CoinType.SUI)
        assertEquals(any.description(), "0x061ce2b2100a71bb7aa0da98998887ad82597948")

        Assert.assertFalse(
            AnyAddress.isValid(
                "0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4",
                CoinType.SUI
            )
        )
    }
}
