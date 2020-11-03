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
}
