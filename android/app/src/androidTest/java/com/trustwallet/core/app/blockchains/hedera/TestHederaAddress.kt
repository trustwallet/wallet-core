// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core.app.blockchains.hedera

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestHederaAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddress() {
        val any = AnyAddress("0.0.1377988", CoinType.HEDERA)
        assertEquals(any.coin(), CoinType.HEDERA)
        assertEquals(any.description(), "0.0.1377988")

        Assert.assertFalse(
            AnyAddress.isValid(
                "0.0.a",
                CoinType.HEDERA
            )
        )
    }
}
