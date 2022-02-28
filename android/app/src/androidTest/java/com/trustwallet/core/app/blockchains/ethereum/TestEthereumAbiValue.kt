package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.EthereumAbiFunction
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.EthereumAbi
import wallet.core.jni.EthereumAbiValue

class TestEthereumAbiValue {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAbiEncoderFuncSimple1() {
        val function = EthereumAbiFunction("sam")
        assertEquals(0, function.addParamBool(true, false))

        assertEquals("sam(bool)", function.type)

        val encoded = EthereumAbi.encode(function)
        assertEquals("0xa35856da" +
            "0000000000000000000000000000000000000000000000000000000000000001",
            Numeric.toHexString(encoded))
    }

    @Test
    fun testEthereumAbiEncoderEncodeFuncCase1() {
        val function = EthereumAbiFunction("sam")
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
        val encoded = EthereumAbi.encode(function)
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
            Numeric.toHexString(encoded))
        // original output value
        assertEquals(0, function.getParamUInt64(0, true))
        // decode output
        val encodedOutput = "0000000000000000000000000000000000000000000000000000000000000045".toHexByteArray()
        val decodeRes = EthereumAbi.decodeOutput(function, encodedOutput)
        assertEquals(true, decodeRes)
        // new output value
        assertEquals(0x45, function.getParamUInt64(0, true))
    }

    @Test
    fun testEthereumAbiValueEncodeInt32() {
        val data = EthereumAbiValue.encodeInt32(69)
        assertEquals(Numeric.toHexString(data), "0x0000000000000000000000000000000000000000000000000000000000000045")
    }

    @Test
    fun testEthereumAbiValueDecodeUInt256() {
        val expected = "1234567890987654321"
        val inputs = listOf(
            "112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb10000000000000000000000000000000000000000000000000000000000000000"
        )
        for (input in inputs) {
            val data = Numeric.hexStringToByteArray(input)
            assertEquals(expected, EthereumAbiValue.decodeUInt256(data))
        }
    }

    @Test
    fun testValueDecoderValue() {
        assertEquals("42", EthereumAbiValue.decodeValue(Numeric.hexStringToByteArray("000000000000000000000000000000000000000000000000000000000000002a"), "uint"))
        assertEquals("24", EthereumAbiValue.decodeValue(Numeric.hexStringToByteArray("0000000000000000000000000000000000000000000000000000000000000018"), "uint8"))
        assertEquals("0xf784682c82526e245f50975190ef0fff4e4fc077", EthereumAbiValue.decodeValue(Numeric.hexStringToByteArray("000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077"), "address"))
        assertEquals("Hello World!    Hello World!    Hello World!", EthereumAbiValue.decodeValue(
            Numeric.hexStringToByteArray("000000000000000000000000000000000000000000000000000000000000002c48656c6c6f20576f726c64212020202048656c6c6f20576f726c64212020202048656c6c6f20576f726c64210000000000000000000000000000000000000000"),
            "string"))
        assertEquals("0x31323334353637383930", EthereumAbiValue.decodeValue(Numeric.hexStringToByteArray("3132333435363738393000000000000000000000000000000000000000000000"), "bytes10"))
    }

    @Test
    fun testValueDecoderArray_uint8() {
        val input = Numeric.hexStringToByteArray("0000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000003100000000000000000000000000000000000000000000000000000000000000320000000000000000000000000000000000000000000000000000000000000033")
        assertEquals("[49,50,51]", EthereumAbiValue.decodeArray(input, "uint8[]"))
    }

    @Test
    fun testValueDecoderArray_address() {
        val input = Numeric.hexStringToByteArray("0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc0770000000000000000000000002e00cd222cb42b616d86d037cc494e8ab7f5c9a3")
        assertEquals("[\"0xf784682c82526e245f50975190ef0fff4e4fc077\",\"0x2e00cd222cb42b616d86d037cc494e8ab7f5c9a3\"]", EthereumAbiValue.decodeArray(input, "address[]"))
    }

    @Test
    fun testValueDecoderArray_bytes() {
        val input = Numeric.hexStringToByteArray("0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000002101100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000031022220000000000000000000000000000000000000000000000000000000000")
        assertEquals("[\"0x1011\",\"0x102222\"]", EthereumAbiValue.decodeArray(input, "bytes[]"))
    }
}
