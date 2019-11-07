package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.EthereumAbiFunction
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.EthereumAbiEncoder

class TestEthereumAbiEncoder {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    val str32_0 = "0000000000000000000000000000000000000000000000000000000000000000"
    val str32_1 = "0000000000000000000000000000000000000000000000000000000000000001"
    val str32_2 = "0000000000000000000000000000000000000000000000000000000000000002"
    val str32_3 = "0000000000000000000000000000000000000000000000000000000000000003"

    @Test
    fun testEthereumAbiEncoderFuncSimple1() {
        val function = EthereumAbiEncoder.buildFunction("sam")
        assertEquals(0, function.addParamBool(true, false))

        assertEquals("sam(bool)", function.type)

        val encoded = EthereumAbiEncoder.encode(function)
        assertEquals("0xa35856da" + str32_1,
            Numeric.toHexString(encoded));
    }

    @Test
    fun testEthereumAbiEncoderEncodeFuncCase1() {
        val function = EthereumAbiEncoder.buildFunction("sam")
        // add params
        assertEquals(0, function.addParamByteArray("0x64617665".toHexByteArray(), false))
        assertEquals(1, function.addParamBool(true, false))
        assertEquals(2, function.addParamArray(false))
        assertEquals(0, function.addInArrayParamUInt256(2, "0x01".toHexByteArray()))
        assertEquals(1, function.addInArrayParamUInt256(2, "0x02".toHexByteArray()))
        assertEquals(2, function.addInArrayParamUInt256(2, "0x03".toHexByteArray()))
        assertEquals(0, function.addParamUInt54(true, true))
        // check signature
        assertEquals("sam(bytes,bool,uint256[])", function.type)
        // encode
        val encoded = EthereumAbiEncoder.encode(function)
        assertEquals("0xa5643bf2" +
            "0000000000000000000000000000000000000000000000000000000000000060" +
            str32_1 +
            "00000000000000000000000000000000000000000000000000000000000000a0" +
            "0000000000000000000000000000000000000000000000000000000000000004" +
            "6461766500000000000000000000000000000000000000000000000000000000" +
            str32_3 + str32_1 + str32_2 + str32_3,
            Numeric.toHexString(encoded));
        // original output value
        assertEquals(0, function.getParamUInt64(function, 0))
        // decode output
        val encodedOutput = "0000000000000000000000000000000000000000000000000000000000000045".toHexByteArray()
        val decodeRes = EthereumAbiEncoder.decodeOutput(decodeOutput, encodedOutput)
        assertEquals(true, decodedRes)
        // new output value
        assertEquals(0x45, function.getParamUInt64(function, 0))
    }
}
