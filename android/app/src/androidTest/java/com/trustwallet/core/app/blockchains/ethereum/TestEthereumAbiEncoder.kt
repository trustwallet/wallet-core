package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.EthereumAbiFunction
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.EthereumAbiEncoder
import wallet.core.jni.EthereumAbiValueEncoder

class TestEthereumAbiEncoder {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAbiEncoderFuncSimple1() {
        val function = EthereumAbiEncoder.buildFunction("sam")
        assertEquals(0, function.addParamBool(true, false))

        assertEquals("sam(bool)", function.type)

        val encoded = EthereumAbiEncoder.encode(function)
        assertEquals("0xa35856da" +
            "0000000000000000000000000000000000000000000000000000000000000001",
            Numeric.toHexString(encoded));
    }

    @Test
    fun testEthereumAbiEncoderEncodeFuncCase1() {
        val function = EthereumAbiEncoder.buildFunction("sam")
        // add params
        assertEquals(0, function.addParamBytes("0x64617665".toHexByteArray(), false))
        assertEquals(1, function.addParamBool(true, false))
        assertEquals(2, function.addParamArray(false))
        assertEquals(0, function.addInArrayParamUInt256(2, "0x01".toHexByteArray()))
        assertEquals(1, function.addInArrayParamUInt256(2, "0x02".toHexByteArray()))
        assertEquals(2, function.addInArrayParamUInt256(2, "0x03".toHexByteArray()))
        assertEquals(0, function.addParamUInt64(0, true))
        // check signature
        assertEquals("sam(bytes,bool,uint256[])", function.type)
        // encode
        val encoded = EthereumAbiEncoder.encode(function)
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
        // original output value
        assertEquals(0, function.getParamUInt64(0, true))
        // decode output
        val encodedOutput = "0000000000000000000000000000000000000000000000000000000000000045".toHexByteArray()
        val decodeRes = EthereumAbiEncoder.decodeOutput(function, encodedOutput)
        assertEquals(true, decodeRes)
        // new output value
        assertEquals(0x45, function.getParamUInt64(0, true))
    }

    @Test
    fun testEthereumAbiValueEncodeInt32() {
        val data = EthereumAbiValueEncoder.encodeInt32(69)
        assertEquals(Numeric.toHexString(data), "0x0000000000000000000000000000000000000000000000000000000000000045")
    }
}
