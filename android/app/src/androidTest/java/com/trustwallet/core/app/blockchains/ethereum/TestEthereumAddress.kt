package com.trustwallet.core.app.blockchains.ethereum

import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import org.junit.Assert.assertFalse

class TestEthereumAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAddresses() {
        val any = AnyAddress("0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1", CoinType.ETHEREUM)
        assertEquals(any.coin(), CoinType.ETHEREUM)
        assertEquals(any.description(), "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1")

        assertFalse(AnyAddress.isValid("0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4", CoinType.ETHEREUM))
    }
}
