// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class EthereumAbiTests: XCTestCase {
    func testAbiEncoder() {
        let function = EthereumAbiFunction(name: "sam")
        // add params
        XCTAssertEqual(0, function.addParamBytes(val: Data(hexString: "0x64617665")!, isOutput: false))
        XCTAssertEqual(1, function.addParamBool(val: true, isOutput: false))
        let idx = function.addParamArray(isOutput: false)
        XCTAssertEqual(2, idx)
        XCTAssertEqual(0, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x01")!))
        XCTAssertEqual(1, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x02")!))
        XCTAssertEqual(2, function.addInArrayParamUInt256(arrayIdx: idx, val: Data(hexString: "0x03")!))
        XCTAssertEqual(0, function.addParamUInt64(val: 0, isOutput: true))

        // check signature
        XCTAssertEqual(function.getType(), "sam(bytes,bool,uint256[])")

        // encode into binrary
        XCTAssertEqual(EthereumAbi.encode(fn: function).hexString, "a5643bf20000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000464617665000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003")

        // original output value
        XCTAssertEqual(0, function.getParamUInt64(idx: 0, isOutput: true))
        // decode output from binary
        let encodedOutput = Data(hexString: "0x0000000000000000000000000000000000000000000000000000000000000045")!
        let decodeRes = EthereumAbi.decodeOutput(fn: function, encoded: encodedOutput)
        XCTAssertEqual(true, decodeRes)
        // new output value
        XCTAssertEqual(0x45, function.getParamUInt64(idx: 0, isOutput: true))
    }

    func testValueEncoder() {
        let data2 = EthereumAbiValue.encodeInt32(value: 69)
        XCTAssertEqual(data2.hexString, "0000000000000000000000000000000000000000000000000000000000000045")
    }

    func testValueDecoderUInt256() {
        let expected = "1234567890987654321"
        let inputs = [
            "112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb1",
            "000000000000000000000000000000000000000000000000112210f4b16c1cb10000000000000000000000000000000000000000000000000000000000000000"
        ]
        for input in inputs {
            let data = Data(hexString: input)!
            XCTAssertEqual(expected, EthereumAbiValue.decodeUInt256(input: data))
        }
    }

    func testValueDecoderValue() {
        XCTAssertEqual("42", EthereumAbiValue.decodeValue(input: Data(hexString: "000000000000000000000000000000000000000000000000000000000000002a")!, type: "uint"))
        XCTAssertEqual("24", EthereumAbiValue.decodeValue(input: Data(hexString: "0000000000000000000000000000000000000000000000000000000000000018")!, type: "uint8"))
        XCTAssertEqual("0xF784682C82526e245F50975190EF0fff4E4fC077", EthereumAbiValue.decodeValue(input: Data(hexString: "000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077")!, type: "address"))
        XCTAssertEqual("Hello World!    Hello World!    Hello World!", EthereumAbiValue.decodeValue(input: Data(hexString:
            "000000000000000000000000000000000000000000000000000000000000002c48656c6c6f20576f726c64212020202048656c6c6f20576f726c64212020202048656c6c6f20576f726c64210000000000000000000000000000000000000000"
            )!, type: "string"))
        XCTAssertEqual("0x31323334353637383930", EthereumAbiValue.decodeValue(input: Data(hexString: "3132333435363738393000000000000000000000000000000000000000000000")!, type: "bytes10"))
    }

    func testValueDecoderArray_uint8() {
        let input = Data(hexString: "0000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000003100000000000000000000000000000000000000000000000000000000000000320000000000000000000000000000000000000000000000000000000000000033")
        XCTAssertEqual("[49,50,51]", EthereumAbiValue.decodeArray(input: input!, type: "uint8[]"))
    }

    func testValueDecoderArray_address() {
        let input = Data(hexString: "0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc0770000000000000000000000002e00cd222cb42b616d86d037cc494e8ab7f5c9a3")
        XCTAssertEqual("[\"0xF784682C82526e245F50975190EF0fff4E4fC077\",\"0x2e00CD222Cb42B616D86D037Cc494e8ab7F5c9a3\"]", EthereumAbiValue.decodeArray(input: input!, type: "address[]"))
    }

    func testValueDecoderArray_bytes() {
        let input = Data(hexString: "0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000002101100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000031022220000000000000000000000000000000000000000000000000000000000")
        XCTAssertEqual("[\"0x1011\",\"0x102222\"]", EthereumAbiValue.decodeArray(input: input!, type: "bytes[]"))
    }

    func testDecodeApprove() throws {
        let data = Data(hexString: "095ea7b30000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed0000000000000000000000000000000000000000000000000000000000000001")!
        let url = Bundle(for: EthereumAbiTests.self).url(forResource: "erc20", withExtension: "json")!
        let abi = try String(contentsOf: url)
        let decoded = EthereumAbi.decodeCall(data: data, abi: abi)!
        let expected = """
        {
            "function": "approve(address,uint256)",
            "inputs": [{
                "name": "_spender",
                "type": "address",
                "value": "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"
            }, {
                "name": "_value",
                "type": "uint256",
                "value": "1"
            }]
        }
        """
        XCTAssertJSONEqual(decoded, expected)
    }

    func testDecode1inch() throws {
        // https://etherscan.io/tx/0xc2d113151124579c21332d4cc6ab2b7f61e81d62392ed8596174513cb47e35ba
        let data = Data(hexString: "7c02520000000000000000000000000027239549dd40e1d60f5b80b0c4196923745b1fd2000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000001800000000000000000000000002b591e99afe9f32eaa6214f7b7629768c40eeb39000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb4800000000000000000000000027239549dd40e1d60f5b80b0c4196923745b1fd20000000000000000000000001611c227725c5e420ef058275ae772b41775e261000000000000000000000000000000000000000000000000000005d0fadb1c0000000000000000000000000000000000000000000000000000000005c31df1da000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000022000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000002080000000000000000000000069d91b94f0aaf8e8a2586909fa77a5c2c89818d50000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000104128acb080000000000000000000000001611c227725c5e420ef058275ae772b41775e2610000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000005d0fadb1c0000000000000000000000000000000000000000000000000000000001000276a400000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000400000000000000000000000002b591e99afe9f32eaa6214f7b7629768c40eeb39000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb4800000000000000000000000000000000000000000000000000000000")!
        let url = Bundle(for: EthereumAbiTests.self).url(forResource: "1inch", withExtension: "json")!
        let abi = try String(contentsOf: url)
        let decoded = EthereumAbi.decodeCall(data: data, abi: abi)!
        let expected = """
        {
            "function": "swap(address,(address,address,address,address,uint256,uint256,uint256,bytes),bytes)",
            "inputs": [{
                "name": "caller",
                "type": "address",
                "value": "0x27239549DD40E1D60F5B80B0C4196923745B1FD2"
            }, {
                "components": [{
                    "name": "srcToken",
                    "type": "address",
                    "value": "0x2b591e99afE9f32eAA6214f7B7629768c40Eeb39"
                }, {
                    "name": "dstToken",
                    "type": "address",
                    "value": "0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48"
                }, {
                    "name": "srcReceiver",
                    "type": "address",
                    "value": "0x27239549DD40E1D60F5B80B0C4196923745B1FD2"
                }, {
                    "name": "dstReceiver",
                    "type": "address",
                    "value": "0x1611C227725c5E420Ef058275AE772b41775e261"
                }, {
                    "name": "amount",
                    "type": "uint256",
                    "value": "6395120000000"
                }, {
                    "name": "minReturnAmount",
                    "type": "uint256",
                    "value": "24748356058"
                }, {
                    "name": "flags",
                    "type": "uint256",
                    "value": "4"
                }, {
                    "name": "permit",
                    "type": "bytes",
                    "value": "0x"
                }],
                "name": "desc",
                "type": "tuple"
            }, {
                "name": "data",
                "type": "bytes",
                "value": "0x00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000002080000000000000000000000069d91b94f0aaf8e8a2586909fa77a5c2c89818d50000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000104128acb080000000000000000000000001611c227725c5e420ef058275ae772b41775e2610000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000005d0fadb1c0000000000000000000000000000000000000000000000000000000001000276a400000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000400000000000000000000000002b591e99afe9f32eaa6214f7b7629768c40eeb39000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb4800000000000000000000000000000000000000000000000000000000"
            }]
        }
        """
        XCTAssertJSONEqual(decoded, expected)
    }

    func testEncodeTyped() throws {
        let message =
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
        """
        let hash = EthereumAbi.encodeTyped(messageJson: message)
        XCTAssertEqual(hash.hexString, "a85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2")
    }

    func testEncodeSeaportMessage() throws {
        let url = Bundle(for: EthereumAbiTests.self).url(forResource: "seaport_712", withExtension: "json")!
        let json = try String(contentsOf: url)
        let hash = EthereumAbi.encodeTyped(messageJson: json)

        XCTAssertEqual(hash.hexString, "54140d99a864932cbc40fd8a2d1d1706c3923a79c183a3b151e929ac468064db")
    }
    
    func testEthereumAbiEncodeFunction() throws {
        let amountIn = EthereumAbiNumberNParam.with {
            $0.bits = 256
            $0.value = Data(hexString: "0x0de0b6b3a7640000")! // 1000000000000000000
        }
        let amountOutMin = EthereumAbiNumberNParam.with {
            $0.bits = 256
            $0.value = Data(hexString: "0x229f7e501ad62bdb")! // 2494851601099271131
        }
        let deadline = EthereumAbiNumberNParam.with {
            $0.bits = 256
            $0.value = Data(hexString: "0x5f0ed070")! // 1594806384
        }
        
        let arr = EthereumAbiArrayParam.with {
            $0.elementType = EthereumAbiParamType.with {
                $0.address = EthereumAbiAddressType.init()
            }
            $0.elements = [
                EthereumAbiToken.with { $0.address = "0x6B175474E89094C44Da98b954EedeAC495271d0F" },
                EthereumAbiToken.with { $0.address = "0x9f8F72aA9304c8B593d555F12eF6589cC3A579A2" },
                EthereumAbiToken.with { $0.address = "0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2" },
                EthereumAbiToken.with { $0.address = "0xE41d2489571d322189246DaFA5ebDe1F4699F498" },
            ]
        }
        let encodingInput = EthereumAbiFunctionEncodingInput.with {
            $0.functionName = "swapExactTokensForTokens"
            $0.tokens = [
                EthereumAbiToken.with { $0.numberUint = amountIn },
                EthereumAbiToken.with { $0.numberUint = amountOutMin },
                EthereumAbiToken.with { $0.array = arr },
                EthereumAbiToken.with { $0.address = "0x7d8bf18C7cE84b3E175b339c4Ca93aEd1dD166F1" },
                EthereumAbiToken.with { $0.numberUint = deadline },
            ]
        }
        
        let inputData = try encodingInput.serializedData()
        let outputData = EthereumAbi.encodeFunction(coin: .ethereum, input: inputData)
        
        let encodingOutput = try EthereumAbiFunctionEncodingOutput(serializedData: outputData)
        XCTAssertEqual(encodingOutput.error, EthereumAbiAbiError.ok)
        XCTAssertEqual(encodingOutput.functionType, "swapExactTokensForTokens(uint256,uint256,address[],address,uint256)")
        
        let expectedEncoded = Data(hexString: "0x38ed17390000000000000000000000000000000000000000000000000de0b6b3a7640000000000000000000000000000000000000000000000000000229f7e501ad62bdb00000000000000000000000000000000000000000000000000000000000000a00000000000000000000000007d8bf18c7ce84b3e175b339c4ca93aed1dd166f1000000000000000000000000000000000000000000000000000000005f0ed07000000000000000000000000000000000000000000000000000000000000000040000000000000000000000006b175474e89094c44da98b954eedeac495271d0f0000000000000000000000009f8f72aa9304c8b593d555f12ef6589cc3a579a2000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2000000000000000000000000e41d2489571d322189246dafa5ebde1f4699f498")!
        XCTAssertEqual(encodingOutput.encoded, expectedEncoded)
    }
    
    func testEthereumAbiDecodeParams() throws {
        let encoded = Data(hexString: "00000000000000000000000088341d1a8f672d2780c8dc725902aae72f143b0c0000000000000000000000000000000000000000000000000000000000000001")!
        
        let abiParams = [
            EthereumAbiParam.with {
                $0.name = "to"
                $0.param = EthereumAbiParamType.with { $0.address = EthereumAbiAddressType.init() }
            },
            EthereumAbiParam.with {
                $0.name = "approved"
                $0.param = EthereumAbiParamType.with { $0.boolean = EthereumAbiBoolType.init() }
            }
        ]
        let decodingInput = EthereumAbiParamsDecodingInput.with {
            $0.encoded = encoded
            $0.abiParams = EthereumAbiAbiParams.with { $0.params = abiParams }
        }
        
        let inputData = try decodingInput.serializedData()
        let outputData = EthereumAbi.decodeParams(coin: .ethereum, input: inputData)
        
        let decodingOutput = try EthereumAbiParamsDecodingOutput(serializedData: outputData)
        XCTAssertEqual(decodingOutput.error, EthereumAbiAbiError.ok)
        
        XCTAssertEqual(decodingOutput.tokens[0].name, "to")
        XCTAssertEqual(decodingOutput.tokens[0].address, "0x88341d1a8F672D2780C8dC725902AAe72F143B0c")
        XCTAssertEqual(decodingOutput.tokens[1].name, "approved")
        XCTAssertEqual(decodingOutput.tokens[1].boolean, true)
    }
    
    func testEthereumAbiDecodeValue() throws {
        let encoded = Data(hexString: "000000000000000000000000000000000000000000000000000000000000002c48656c6c6f20576f726c64212020202048656c6c6f20576f726c64212020202048656c6c6f20576f726c64210000000000000000000000000000000000000000")!

        let decodingInput = EthereumAbiValueDecodingInput.with {
            $0.encoded = encoded
            $0.paramType = "string"
        }
        
        let inputData = try decodingInput.serializedData()
        let outputData = EthereumAbi.decodeValue(coin: .ethereum, input: inputData)
        
        let decodingOutput = try EthereumAbiValueDecodingOutput(serializedData: outputData)
        XCTAssertEqual(decodingOutput.error, EthereumAbiAbiError.ok)
        XCTAssertEqual(decodingOutput.token.stringValue, "Hello World!    Hello World!    Hello World!")
    }
    
    func testEthereumAbiDecodeContractCall() throws {
        let encoded = Data(hexString: "c47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000086465616462656566000000000000000000000000000000000000000000000000")!
        let abiJson = """
            {
                "c47f0027": {
                    "constant": false,
                    "inputs": [
                        {
                            "name": "name",
                            "type": "string"
                        }
                    ],
                    "name": "setName",
                    "outputs": [],
                    "payable": false,
                    "stateMutability": "nonpayable",
                    "type": "function"
                }
            }
        """
        
        let decodingInput = EthereumAbiContractCallDecodingInput.with {
            $0.encoded = encoded
            $0.smartContractAbiJson = abiJson
        }
        
        let inputData = try decodingInput.serializedData()
        let outputData = EthereumAbi.decodeContractCall(coin: .ethereum, input: inputData)
        
        let decodingOutput = try EthereumAbiContractCallDecodingOutput(serializedData: outputData)
        XCTAssertEqual(decodingOutput.error, EthereumAbiAbiError.ok)
        
        let expectedJson = #"{"function":"setName(string)","inputs":[{"name":"name","type":"string","value":"deadbeef"}]}"#
        XCTAssertEqual(decodingOutput.decodedJson, expectedJson)
        XCTAssertEqual(decodingOutput.tokens[0].name, "name")
        XCTAssertEqual(decodingOutput.tokens[0].stringValue, "deadbeef")
    }
}
