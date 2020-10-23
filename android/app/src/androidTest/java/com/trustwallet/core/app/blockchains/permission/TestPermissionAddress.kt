// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.permission

import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.*

class TestPermissionAddress {

  init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAddresses() {
        val any = AnyAddress("0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", CoinType.PERMISSION)
        assertEquals(any.coin(), CoinType.PERMISSION)
        assertEquals(any.description(), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")

        assertFalse(AnyAddress.isValid("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4", CoinType.PERMISSION))
    }
}
