// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        XCTAssertEqual("0xf784682c82526e245f50975190ef0fff4e4fc077", EthereumAbiValue.decodeValue(input: Data(hexString: "000000000000000000000000f784682c82526e245f50975190ef0fff4e4fc077")!, type: "address"))
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
        XCTAssertEqual("[\"0xf784682c82526e245f50975190ef0fff4e4fc077\",\"0x2e00cd222cb42b616d86d037cc494e8ab7f5c9a3\"]", EthereumAbiValue.decodeArray(input: input!, type: "address[]"))
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
                "value": "0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed"
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
                "value": "0x27239549dd40e1d60f5b80b0c4196923745b1fd2"
            }, {
                "components": [{
                    "name": "srcToken",
                    "type": "address",
                    "value": "0x2b591e99afe9f32eaa6214f7b7629768c40eeb39"
                }, {
                    "name": "dstToken",
                    "type": "address",
                    "value": "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"
                }, {
                    "name": "srcReceiver",
                    "type": "address",
                    "value": "0x27239549dd40e1d60f5b80b0c4196923745b1fd2"
                }, {
                    "name": "dstReceiver",
                    "type": "address",
                    "value": "0x1611c227725c5e420ef058275ae772b41775e261"
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
}
