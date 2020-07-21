package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.EthereumAbi

class TestEthereumAbiDecoder {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testEthereumAbiDecodeApprove() {
        val call = "095ea7b30000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed0000000000000000000000000000000000000000000000000000000000000001".toHexByteArray()
        val abi = """
            {
                "095ea7b3": {
                    "constant": false,
                    "inputs": [{
                        "name": "_spender",
                        "type": "address"
                    }, {
                        "name": "_value",
                        "type": "uint256"
                    }],
                    "name": "approve",
                    "outputs": [],
                    "payable": false,
                    "stateMutability": "nonpayable",
                    "type": "function"
                }
            }
        """.trimIndent()

        val decoded = EthereumAbi.decodeCall(call, abi)
        val expected = """{"function":"approve(address,uint256)","inputs":[{"name":"_spender","type":"address","value":"0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed"},{"name":"_value","type":"uint256","value":"1"}]}"""

        assertEquals(decoded, expected)
    }
}
