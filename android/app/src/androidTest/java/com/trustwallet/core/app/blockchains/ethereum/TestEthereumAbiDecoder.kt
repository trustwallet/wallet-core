package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.EthereumAbiValueDecoder

class TestEthereumAbiDecoder {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAbiDecoder() {
        val expected = "1234567890987654321"
        val inputs = listOf(
            "112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb10000000000000000000000000000000000000000000000000000000000000000"
        )
        for (input in inputs) {
            val data = Numeric.hexStringToByteArray(input)
            assertEquals(expected, EthereumAbiValueDecoder.decodeUInt256(data))
        }
    }
}
