package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
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

    @Test
    fun testEthereumAbiEncodeTyped() {
        val hash = EthereumAbi.encodeTyped(
        """
            {
                "types": {
                    "EIP712Domain": [
                        {"name": "name", "type": "string"},
                        {"name": "version", "type": "string"},
                        {"name": "chainId", "type": "uint256"},
                        {"name": "verifyingContract", "type": "address"}
                    ],
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallets", "type": "address[]"}
                    ],
                    "Mail": [
                        {"name": "from", "type": "Person"},
                        {"name": "to", "type": "Person[]"},
                        {"name": "contents", "type": "string"}
                    ]
                },
                "primaryType": "Mail",
                "domain": {
                    "name": "Ether Mail",
                    "version": "1",
                    "chainId": 1,
                    "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
                },
                "message": {
                    "from": {
                        "name": "Cow",
                        "wallets": [
                            "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
                            "DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
                        ]
                    },
                    "to": [
                        {
                            "name": "Bob",
                            "wallets": [
                                "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
                                "B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57",
                                "B0B0b0b0b0b0B000000000000000000000000000"
                            ]
                        }
                    ],
                    "contents": "Hello, Bob!"
                }
            }
        """)
        assertEquals(Numeric.toHexString(hash), "0xa85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2")
    }
}
