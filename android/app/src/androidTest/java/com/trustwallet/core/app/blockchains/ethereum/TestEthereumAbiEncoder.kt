package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.EthAbiFunction
import com.trustwallet.core.app.utils.Numeric

class TestEthereumAbiEncoder {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumABIEncoderFuncSimple1() {
        val encoder = EthAbiFunction("sam")
        assertEquals(0, encoder.addInParamBool(true))

        assertEquals("sam(bool)", encoder.type)

        val encoded = encoder.encode()
        assertEquals("0xa35856da" +
            "0000000000000000000000000000000000000000000000000000000000000001",
            Numeric.toHexString(encoded));
    }

    @Test
    fun testEthereumABIEncoderEncodeFuncCase1() {
        val encoder = EthAbiFunction("sam")

        assertEquals(0, encoder.addInParamByteArray("0x64617665".toHexByteArray()))
        assertEquals(1, encoder.addInParamBool(true))
        assertEquals(2, encoder.addInParamArray())
        assertEquals(0, encoder.addInArrayParamUInt256(2, "0x01".toHexByteArray()))
        assertEquals(1, encoder.addInArrayParamUInt256(2, "0x02".toHexByteArray()))
        assertEquals(2, encoder.addInArrayParamUInt256(2, "0x03".toHexByteArray()))

        assertEquals("sam(bytes,bool,uint256[])", encoder.type)

        val encoded = encoder.encode()
        assertEquals("0xa5643bf2" +
            "0000000000000000000000000000000000000000000000000000000000000060" +
            "0000000000000000000000000000000000000000000000000000000000000001" +
            "00000000000000000000000000000000000000000000000000000000000000a0" +
            "0000000000000000000000000000000000000000000000000000000000000004" +
            "6461766500000000000000000000000000000000000000000000000000000000" +
            "0000000000000000000000000000000000000000000000000000000000000003" +
            "0000000000000000000000000000000000000000000000000000000000000001" +
            "0000000000000000000000000000000000000000000000000000000000000002" +
            "0000000000000000000000000000000000000000000000000000000000000003",
            Numeric.toHexString(encoded));
    }
}
