package com.trustwallet.core.app.blockchains.ethereum

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import org.junit.Assert.assertEquals
import org.junit.Test
import com.trustwallet.core.app.utils.Numeric
import wallet.core.jni.CoinType
import wallet.core.jni.proto.EthereumAbi

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

        val decoded = wallet.core.jni.EthereumAbi.decodeCall(call, abi)
        val expected = """{"function":"approve(address,uint256)","inputs":[{"name":"_spender","type":"address","value":"0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"},{"name":"_value","type":"uint256","value":"1"}]}"""

        assertEquals(decoded, expected)
    }

    @Test
    fun testEthereumAbiEncodeTyped() {
        val hash = wallet.core.jni.EthereumAbi.encodeTyped(
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

    @Test
    fun testEthereumAbiEncodeFunction() {
        val amountIn = EthereumAbi.NumberNParam.newBuilder().apply {
            bits = 256
            value = ByteString.copyFrom(Numeric.hexStringToByteArray("0xde0b6b3a7640000")) // 1000000000000000000
        }
        val amountOutMin = EthereumAbi.NumberNParam.newBuilder().apply {
            bits = 256
            value = ByteString.copyFrom(Numeric.hexStringToByteArray("0x229f7e501ad62bdb")) // 2494851601099271131
        }
        val deadline = EthereumAbi.NumberNParam.newBuilder().apply {
            bits = 256
            value = ByteString.copyFrom(Numeric.hexStringToByteArray("0x5f0ed070")) // 1594806384
        }
        val addressType = EthereumAbi.AddressType.newBuilder().build()

        val encodingInput = EthereumAbi.FunctionEncodingInput.newBuilder()
            .setFunctionName("swapExactTokensForTokens")
            .addTokens(EthereumAbi.Token.newBuilder().setNumberUint(amountIn))
            .addTokens(EthereumAbi.Token.newBuilder().setNumberUint(amountOutMin))
            .addTokens(EthereumAbi.Token.newBuilder().apply {
                array = EthereumAbi.ArrayParam.newBuilder()
                    .setElementType(EthereumAbi.ParamType.newBuilder().setAddress(addressType))
                    .addElements(EthereumAbi.Token.newBuilder().setAddress("0x6B175474E89094C44Da98b954EedeAC495271d0F"))
                    .addElements(EthereumAbi.Token.newBuilder().setAddress("0x9f8F72aA9304c8B593d555F12eF6589cC3A579A2"))
                    .addElements(EthereumAbi.Token.newBuilder().setAddress("0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2"))
                    .addElements(EthereumAbi.Token.newBuilder().setAddress("0xE41d2489571d322189246DaFA5ebDe1F4699F498"))
                    .build()
            })
            .addTokens(EthereumAbi.Token.newBuilder().setAddress("0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1"))
            .addTokens(EthereumAbi.Token.newBuilder().setNumberUint(deadline))
            .build()

        val encodingOutputData = wallet.core.jni.EthereumAbi.encodeFunction(CoinType.ETHEREUM, encodingInput.toByteArray())
        val encodingOutput = EthereumAbi.FunctionEncodingOutput.parseFrom(encodingOutputData)

        assertEquals(encodingOutput.error, EthereumAbi.AbiError.OK)
        assertEquals(encodingOutput.functionType, "swapExactTokensForTokens(uint256,uint256,address[],address,uint256)")

        val expectedEncoded = ByteString.copyFrom(Numeric.hexStringToByteArray("0x38ed17390000000000000000000000000000000000000000000000000de0b6b3a7640000000000000000000000000000000000000000000000000000229f7e501ad62bdb00000000000000000000000000000000000000000000000000000000000000a00000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f1000000000000000000000000000000000000000000000000000000005f0ed07000000000000000000000000000000000000000000000000000000000000000040000000000000000000000006b175474e89094c44da98b954eedeac495271d0f0000000000000000000000009f8f72aa9304c8b593d555f12ef6589cc3a579a2000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2000000000000000000000000e41d2489571d322189246dafa5ebde1f4699f498"))
        assertEquals(encodingOutput.encoded, expectedEncoded)
    }

    @Test
    fun testEthereumAbiDecodeParams() {
        val encoded = ByteString.copyFrom(Numeric.hexStringToByteArray("00000000000000000000000088341d1a8f672d2780c8dc725902aae72f143b0c0000000000000000000000000000000000000000000000000000000000000001"))

        val addressType = EthereumAbi.AddressType.newBuilder().build()
        val boolType = EthereumAbi.BoolType.newBuilder().build()

        val params = EthereumAbi.AbiParams.newBuilder()
            .addParams(EthereumAbi.Param.newBuilder().apply {
                name = "to"
                param = EthereumAbi.ParamType.newBuilder().setAddress(addressType).build()
            })
            .addParams(EthereumAbi.Param.newBuilder().apply {
                name = "approved"
                param = EthereumAbi.ParamType.newBuilder().setBoolean(boolType).build()
            })
        val decodingInput = EthereumAbi.ParamsDecodingInput.newBuilder()
            .setEncoded(encoded)
            .setAbiParams(params)
            .build()

        val decodingOutputData = wallet.core.jni.EthereumAbi.decodeParams(CoinType.ETHEREUM, decodingInput.toByteArray())
        val decodingOutput = EthereumAbi.ParamsDecodingOutput.parseFrom(decodingOutputData)

        assertEquals(decodingOutput.error, EthereumAbi.AbiError.OK)

        assertEquals(decodingOutput.getTokens(0).name, "to")
        assertEquals(decodingOutput.getTokens(0).address, "0x88341d1a8F672D2780C8dC725902AAe72F143B0c")

        assertEquals(decodingOutput.getTokens(1).name, "approved")
        assertEquals(decodingOutput.getTokens(1).boolean, true)
    }

    @Test
    fun testEthereumAbiDecodeValue() {
        val encoded = ByteString.copyFrom(Numeric.hexStringToByteArray("000000000000000000000000000000000000000000000000000000000000002c48656c6c6f20576f726c64212020202048656c6c6f20576f726c64212020202048656c6c6f20576f726c64210000000000000000000000000000000000000000"))

        val decodingInput = EthereumAbi.ValueDecodingInput.newBuilder()
            .setEncoded(encoded)
            .setParamType("string")
            .build()

        val decodingOutputData = wallet.core.jni.EthereumAbi.decodeValue(CoinType.ETHEREUM, decodingInput.toByteArray())
        val decodingOutput = EthereumAbi.ValueDecodingOutput.parseFrom(decodingOutputData)

        assertEquals(decodingOutput.error, EthereumAbi.AbiError.OK)
        assertEquals(decodingOutput.token.stringValue, "Hello World!    Hello World!    Hello World!")
    }

    @Test
    fun testEthereumAbiDecodeContractCall() {
        val encoded = ByteString.copyFrom(Numeric.hexStringToByteArray("c47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000086465616462656566000000000000000000000000000000000000000000000000"))
        val abi = """{"c47f0027":{"constant":false,"inputs":[{"name":"name","type":"string"}],"name":"setName","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"}}"""
        val decodingInput = EthereumAbi.ContractCallDecodingInput.newBuilder()
            .setEncoded(encoded)
            .setSmartContractAbiJson(abi)
            .build()

        val decodingOutputData = wallet.core.jni.EthereumAbi.decodeContractCall(CoinType.ETHEREUM, decodingInput.toByteArray())
        val decodingOutput = EthereumAbi.ContractCallDecodingOutput.parseFrom(decodingOutputData)

        assertEquals(decodingOutput.error, EthereumAbi.AbiError.OK)

        val expectedJson = """{"function":"setName(string)","inputs":[{"name":"name","type":"string","value":"deadbeef"}]}"""
        assertEquals(decodingOutput.decodedJson, expectedJson)

        assertEquals(decodingOutput.getTokens(0).name, "name")
        assertEquals(decodingOutput.getTokens(0).stringValue, "deadbeef")
    }
}
