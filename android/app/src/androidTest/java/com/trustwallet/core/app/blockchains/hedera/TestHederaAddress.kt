// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
